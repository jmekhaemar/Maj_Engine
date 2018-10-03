#include "physic/Instance.h"

#include <typeinfo>
#include <common/PxTolerancesScale.h>
#include <pvd/PxPvd.h>

#include <JsonNode.h>

#include "scene/Actor.h"
#include "physic/PhysXHelper.h"

#include <thread>

#define PVD_HOST "127.0.0.1"

namespace maj::physic
{
	PhysXErrorCallback Instance::sErrorCallback {};
	PhysXSimulationCallback Instance::sSimulationCallback {};
	physx::PxDefaultAllocator Instance::sDefaultAllocatorCallback {};
	LayerFilter Instance::sLayerFilter {};

	LayerFilter& Instance::layerFilter()
	{
		return sLayerFilter;
	}

	physx::PxFilterFlags Instance::filterShader(physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0, physx::PxFilterObjectAttributes attributes1,
												physx::PxFilterData filterData1, physx::PxPairFlags& pairFlags, const void* kConstantBlock, physx::PxU32 constantBlockSize)
	{
		(void) constantBlockSize;
		(void) kConstantBlock;

		if ((static_cast<int32_t>(sLayerFilter.filter(static_cast<ELayer>(filterData0.word0)) & static_cast<ELayer>(filterData1.word0)) != 0u) && (static_cast<int32_t>(sLayerFilter.filter(static_cast<ELayer>(filterData1.word0)) & static_cast<ELayer>(filterData0.word0)) != 0u))
		{
			pairFlags = physx::PxPairFlag::eDETECT_DISCRETE_CONTACT | physx::PxPairFlag::eSOLVE_CONTACT;

			if (physx::PxFilterObjectIsTrigger(attributes0) || physx::PxFilterObjectIsTrigger(attributes1))
			{
				pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_FOUND | physx::PxPairFlag::eNOTIFY_TOUCH_LOST;
			}
			else
			{
				pairFlags |= physx::PxPairFlag::eNOTIFY_CONTACT_POINTS | physx::PxPairFlag::eNOTIFY_THRESHOLD_FORCE_FOUND | physx::PxPairFlag::eNOTIFY_THRESHOLD_FORCE_LOST
					| physx::PxPairFlag::eNOTIFY_THRESHOLD_FORCE_PERSISTS;
			}

			return physx::PxFilterFlag::eDEFAULT;
		}

		return physx::PxFilterFlag::eSUPPRESS;
	}

	Instance::Instance()
		: Service(typeid(Instance).name()), mFoundation { nullptr }, mPvd { nullptr }, mPhysics { nullptr }, mScene { nullptr }
	{
		mFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, sDefaultAllocatorCallback, sErrorCallback);
		if (!mFoundation)
			THROW("failed to create physic instance: failed to PxCreateFoundation")

		mPvd = PxCreatePvd(*mFoundation);
		physx::PxPvdTransport* transport { physx::PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10) };
		mPvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);

		mPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation, physx::PxTolerancesScale(), true, mPvd);
		if (!mPhysics)
			THROW("failed to create physic instance: failed to PxCreatePhysics")

		if (!PxInitExtensions(*mPhysics, mPvd))
			THROW("failed to create physic instance: failed to PxInitExtensions");
		
		physx::PxSceneDesc sceneDesc { mPhysics->getTolerancesScale() };
		sceneDesc.gravity = physx::PxVec3{ 0.f, -9.8f, 0.f };

		if (!sceneDesc.cpuDispatcher)
		{
			physx::PxDefaultCpuDispatcher* cpuDispatcher { physx::PxDefaultCpuDispatcherCreate(std::thread::hardware_concurrency()) };
			if (!cpuDispatcher)
				THROW("failed to create physic instance: failed to PxDefaultCpuDispatcherCreate")

			sceneDesc.cpuDispatcher = cpuDispatcher;
		}

		if (!sceneDesc.filterShader)
			sceneDesc.filterShader = filterShader;

		sceneDesc.simulationEventCallback = &sSimulationCallback;

		mScene = mPhysics->createScene(sceneDesc);
		if (!mScene)
			THROW("failed to create physic instance: failed to create scene")

		mScene->setVisualizationParameter(physx::PxVisualizationParameter::eSCALE, 1.f);
		mScene->setVisualizationParameter(physx::PxVisualizationParameter::eCOLLISION_SHAPES, 1.f);

		try
		{
			std::unique_ptr<JsonNode> root { JsonNode::parse("Layer.config") };
			sLayerFilter.load(root.get());
		}
		catch (const std::exception& e)
		{
			LOG(LogPhysicWarning, "failed to load layer config: " << e.what())
		}
	}

	Instance::~Instance()
	{
		sLayerFilter.save("Layer.config");

		if (mScene)
			mScene->release();

		PxCloseExtensions();

		if (mPhysics)
			mPhysics->release();

		if (mPvd)
			mPvd->release();

		if (mFoundation)
			mFoundation->release();
	}

	physx::PxRigidActor* Instance::createRigidActor(scene::Actor* actor)
	{
		if (!actor)
			THROW("failed to create rigid actor: no actor")

		const scene::CTransform* kTransf { actor->rootTransform() };
		if (!kTransf)
			THROW("failed to create rigid actor: actor \'" + actor->name() + " has no root transform")
	
		physx::PxTransform globalTr { convert(kTransf->global()) };

		physx::PxRigidActor* rigidActor { nullptr };
		if (kTransf->isStatic())
			rigidActor = mPhysics->createRigidStatic(globalTr);
		else
		{
			physx::PxRigidDynamic* dynActor { mPhysics->createRigidDynamic(globalTr) };
			if (dynActor)
			{
				dynActor->setRigidBodyFlag(physx::PxRigidBodyFlag::eENABLE_POSE_INTEGRATION_PREVIEW, true);
				dynActor->setContactReportThreshold(math::Helper<float>::skEpsilon);
			}

			rigidActor = dynActor;
		}

		if (!rigidActor)
			THROW("failed to create rigid actor: failed to create physical actor")

		rigidActor->userData = actor;
		mScene->addActor(*rigidActor);

		return rigidActor;
	}

	physx::PxShape* Instance::createShape(physx::PxRigidActor* rigidActor, const physx::PxGeometry& kGeometry, const PhysicMaterial& kMaterial)
	{
		if (!rigidActor)
			THROW("failed to create shape: no rigid actor")
			
		physx::PxShape* shape { mPhysics->createShape(kGeometry, kMaterial.material(), true) };
		if (!shape)
			THROW("failed to create shape: failed to create physical shape")

		rigidActor->attachShape(*shape);
		shape->release();

		return shape;
	}

	physx::PxMaterial* Instance::createMaterial(float staticFriction, float dynamicFriction, float restitution)
	{
		physx::PxMaterial* material { mPhysics->createMaterial(staticFriction, dynamicFriction, restitution) };
		if (!material)
			THROW("failed to create material: failed to create physical material")

		return material;
	}

	const physx::PxScene* Instance::scene() const
	{
		return mScene;
	}

	physx::PxScene* Instance::scene()
	{
		return mScene;
	}
}