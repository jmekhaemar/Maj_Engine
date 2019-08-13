#include "resources/LoaderFbx.h"

#include <unordered_map>
#include "anim/Joint.h"

namespace maj::resources
{
	LoaderFbx::LoaderFbx()
		: Service { typeid(LoaderFbx).name() }, mScene { nullptr }, mManager { nullptr }, mHasAnimation { true }, mControlPoints {}, mTriangles {}
	{}

	LoaderFbx::~LoaderFbx()
	{
		if (mScene)
			mScene->Destroy();

		if (mManager)
			mManager->Destroy();
	}

	void LoaderFbx::load(const char* kPath, std::vector<anim::Vertex>& vertices, std::vector<uint32_t>& indices, anim::Skeleton* skull,
						 std::vector<anim::Animation>& animations)
	{
		mTriangles.clear();
		mControlPoints.clear();
		mHasAnimation = true;
		parseFbx(kPath, vertices, indices, skull, animations);
	}

	// Loading/Parsing nodes

	void LoaderFbx::initFbx(const char* kPath)
	{
		if (!mManager)
		{
			mManager = FbxManager::Create();
			FbxIOSettings *ioSettings { FbxIOSettings::Create(mManager, IOSROOT) };
			mManager->SetIOSettings(ioSettings);
		}

		if (mScene)
			mScene->Destroy();

		FbxImporter* importer { FbxImporter::Create(mManager, "") };
		bool status { importer->Initialize(kPath, -1, mManager->GetIOSettings()) };

		if (!status)
			THROW("importer initializer failed with error: " + std::string { importer->GetStatus().GetErrorString() })

		mScene = FbxScene::Create(mManager, "Scene");
		status = importer->Import(mScene);

		if (!status)
			THROW("import failed with error: " + std::string { importer->GetStatus().GetErrorString() })

		importer->Destroy();

		FbxGeometryConverter geometryConverter { mManager };
		geometryConverter.Triangulate(mScene, true);

		FbxAxisSystem sceneAxisSystem { mScene->GetGlobalSettings().GetAxisSystem() };
		FbxAxisSystem axisSystem { FbxAxisSystem::eYAxis, FbxAxisSystem::eParityOdd, FbxAxisSystem::eRightHanded };
		if (sceneAxisSystem != axisSystem)
			axisSystem.ConvertScene(mScene);

		FbxSystemUnit sceneUnitSystem { mScene->GetGlobalSettings().GetSystemUnit() };

		if (sceneUnitSystem == FbxSystemUnit::cm)
		{
			FbxSystemUnit unitSystem { 100.f };
			unitSystem.ConvertScene(mScene);
		}

		if (!math::Helper<double>::areSame(sceneUnitSystem.GetScaleFactor(), 1.0))
		{
			FbxSystemUnit unitSystem { 1.0f };
			unitSystem.ConvertScene(mScene);
		}
	}

	void LoaderFbx::parseFbx(const char* kPath, std::vector<anim::Vertex>& vertices, std::vector<uint32_t>& indices, anim::Skeleton* skull,
							 std::vector<anim::Animation>& animations)
	{
		std::vector<anim::Vertex> tempVertices {};
		initFbx(kPath);

		FbxNode* node { mScene->GetRootNode() };
		
		processSkeletonHierarchy(node, skull);
		mHasAnimation = skull->jointCount() != 0u;

		processNode(node, tempVertices, indices, skull, animations);
		optimize(tempVertices, vertices, indices);
	}

	void LoaderFbx::processNode(FbxNode *node, std::vector<anim::Vertex>& vertices, std::vector<uint32_t>& indices, anim::Skeleton* skull, std::vector<anim::Animation>& animations)
	{
		FbxNodeAttribute* attribute { node->GetNodeAttribute() };
		if (attribute)
		{
			switch (attribute->GetAttributeType())
			{
				case FbxNodeAttribute::eMesh:
				{
					processControlPoints(node);
					if (mHasAnimation)
						processJointsAndAnimations(node, skull, animations);

					addMesh(node, vertices);
				}
				break;

				default:
				break;
			}
		}
		
		uint32_t childCount { static_cast<uint32_t>(node->GetChildCount()) };
		for (uint32_t i{ 0u }; i < childCount ; ++i)
			processNode(node->GetChild(i), vertices, indices, skull, animations);
	}

	void LoaderFbx::addMesh(FbxNode* node, std::vector<anim::Vertex>& vertices)
	{
		FbxMesh* currMesh { node->GetMesh() };
		if (!currMesh || !currMesh->IsTriangleMesh())
			return;

		currMesh->RemoveBadPolygons();

		FbxLayerElementTangent* meshTangents{ currMesh->GetElementTangent(0) };
		if (!meshTangents && currMesh->GenerateTangentsDataForAllUVSets(true))
			meshTangents = currMesh->GetElementTangent(0);

		FbxAMatrix finalGlobalTransform{ globalMatrix(node) };
		
		uint32_t triangleCount { static_cast<uint32_t>(currMesh->GetPolygonCount()) };
		uint32_t vertexCounter { 0u };
		mTriangles.reserve(triangleCount);

		for (uint32_t i { 0u }; i < triangleCount; ++i)
		{
			math::Vec3Holder<float> normal[3u] {};
			math::Vec3Holder<float> tangent[3u] {};
			math::Vec2Holder<float> uv[3u][2u] {};

			for (uint32_t j{ 0u } ; j < 3u ; ++j)
			{
				uint32_t ctrlPointIndex { static_cast<uint32_t>(currMesh->GetPolygonVertex(i, j)) };
				const anim::ControlPoint& kCtrlPoint { mControlPoints[ctrlPointIndex] };

				readNormal(currMesh, normal[j], ctrlPointIndex, vertexCounter, finalGlobalTransform);

				for (uint32_t k{ 0u }; k < MAX_UV_SET; ++k)
					readUv(currMesh, uv[j][k], i, j, k);

				readTangent(currMesh, tangent[j], ctrlPointIndex, vertexCounter, finalGlobalTransform);

				// This should be changed if more than 1 uv set
				anim::Vertex temp { kCtrlPoint.mPosition, uv[j][0u], normal[j], tangent[j]};

				if (mHasAnimation)
				{
					math::Vec4Holder<float> boneIndex	{	kCtrlPoint.mBlendingInfo[0u].mBlendingIndex,
												kCtrlPoint.mBlendingInfo[1u].mBlendingIndex,
												kCtrlPoint.mBlendingInfo[2u].mBlendingIndex,
												kCtrlPoint.mBlendingInfo[3u].mBlendingIndex };

					math::Vec4Holder<float> boneWeight	{	kCtrlPoint.mBlendingInfo[0u].mBlendingWeight,
												kCtrlPoint.mBlendingInfo[1u].mBlendingWeight,
												kCtrlPoint.mBlendingInfo[2u].mBlendingWeight,
												kCtrlPoint.mBlendingInfo[3u].mBlendingWeight };

					temp.boneIndex(boneIndex);
					temp.boneWeight(boneWeight);
				}

				vertices.emplace_back(temp);
				mTriangles.emplace_back(vertexCounter);
				++vertexCounter;
			}
		}

		mControlPoints.clear();
	}

	// Parsing attributes

	void LoaderFbx::processControlPoints(FbxNode* node)
	{
		FbxMesh* currMesh { node->GetMesh() };
		uint32_t ctrlPointCount { static_cast<uint32_t>(currMesh->GetControlPointsCount()) };

		FbxAMatrix finalGlobalTransform { globalMatrix(node) };
		mControlPoints.reserve(ctrlPointCount);

		for (uint32_t i { 0u }; i < ctrlPointCount; ++i)
		{
			anim::ControlPoint currCtrlPoint {};

			FbxVector4 position { currMesh->GetControlPointAt(i) };
			position = finalGlobalTransform.MultT(position);

			currCtrlPoint.mPosition = { static_cast<float>(position.mData[0]), static_cast<float>(position.mData[1]), static_cast<float>(position.mData[2]) };
			mControlPoints.emplace_back(std::move(currCtrlPoint));
		}
	}

	void LoaderFbx::readUv(FbxMesh* mesh, math::Vec2Holder<float>& uv, uint32_t indexI, uint32_t indexJ, uint32_t uvLayer)
	{
		uint32_t ctrlPointIndex {0u};

		if (uvLayer >= 2u || mesh->GetElementUVCount() <= static_cast<int32_t>(uvLayer))
			THROW("failed to read UV: invalid UV layer count")

		FbxGeometryElementUV* vertexUV{ mesh->GetElementUV(uvLayer) };
		if(!vertexUV)
			THROW("failed to read UV: invalid FbxGeometryElementUV")

		FbxGeometryElementUV::EMappingMode mappingMode { vertexUV->GetMappingMode() };
		FbxGeometryElementUV::EReferenceMode refMode { vertexUV->GetReferenceMode() };
		
		FbxVector2 tempUV {};
		switch (mappingMode)
		{
			case FbxGeometryElementUV::EMappingMode::eByControlPoint:
			{
				ctrlPointIndex = mesh->GetPolygonVertex(indexI, indexJ);
				switch (refMode)
				{
					case FbxGeometryElementUV::EReferenceMode::eDirect:
					{
						tempUV = vertexUV->GetDirectArray().GetAt(ctrlPointIndex);
					}
					break;

					case FbxGeometryElementUV::EReferenceMode::eIndexToDirect:
					{
						int32_t index = vertexUV->GetIndexArray().GetAt(ctrlPointIndex);
						tempUV = vertexUV->GetDirectArray().GetAt(index);
					}
					break;

					default:
					THROW("failed to read UV: unhandled reference mode")

				}
			}
			break;

			case FbxGeometryElementUV::EMappingMode::eByPolygonVertex:
			{
				switch (refMode)
				{
					case FbxGeometryElementUV::EReferenceMode::eDirect:
					case FbxGeometryElementUV::EReferenceMode::eIndexToDirect:
					{
						ctrlPointIndex = mesh->GetTextureUVIndex(indexI, indexJ);
						tempUV = vertexUV->GetDirectArray().GetAt(ctrlPointIndex);
					}
					break;

					default:
					THROW("failed to read UV: unhandled reference mode")
				}
			}
			break;

			default:
			THROW("failed to read UV: unhandled mapping mode")
		}

		uv.mX = static_cast<float>(tempUV[0]);
		uv.mY = static_cast<float>(-tempUV[1]);
	}
	
	void LoaderFbx::readNormal(FbxMesh* mesh, math::Vec3Holder<float>& normal, uint32_t ctrlPointIndex, uint32_t vertexCounter, const FbxAMatrix& kFinalGlobalTransform)
	{
		if (mesh->GetElementNormalCount() < 1)
			THROW("failed to read normal: invalid normal count")

		FbxGeometryElementNormal* vertexNormal { mesh->GetElementNormal(0) };
		if (!vertexNormal)
			THROW("failed to read UV: invalid FbxGeometryElementNormal")

		FbxGeometryElementNormal::EMappingMode mappingMode { vertexNormal->GetMappingMode() };
		FbxGeometryElementNormal::EReferenceMode refMode { vertexNormal->GetReferenceMode() };

		FbxVector4 tempNormal {};
		switch (mappingMode)
		{
			case FbxGeometryElementNormal::EMappingMode::eByControlPoint:
			{
				switch (refMode)
				{
					case FbxGeometryElementNormal::EReferenceMode::eDirect:
					{
						tempNormal = vertexNormal->GetDirectArray().GetAt(ctrlPointIndex);
					}
					break;

					case FbxGeometryElementNormal::EReferenceMode::eIndexToDirect:
					{
						uint32_t index { static_cast<uint32_t>(vertexNormal->GetIndexArray().GetAt(ctrlPointIndex)) };
						tempNormal = vertexNormal->GetDirectArray().GetAt(index);
					}
					break;

					default:
					THROW("failed to read normal: unhandled reference mode")
				}
			}
			break;

			case FbxGeometryElementNormal::EMappingMode::eByPolygonVertex:
			{
				switch (refMode)
				{
					case FbxGeometryElementNormal::EReferenceMode::eDirect:
					{
						tempNormal = vertexNormal->GetDirectArray().GetAt(vertexCounter);
					}
					break;

					case FbxGeometryElementNormal::EReferenceMode::eIndexToDirect:
					{
						uint32_t index { static_cast<uint32_t>(vertexNormal->GetIndexArray().GetAt(vertexCounter)) };
						tempNormal = vertexNormal->GetDirectArray().GetAt(index);
					}
					break;

					default:
					THROW("failed to read normal: unhandled reference mode")
				}
			}
			break;

			default:
			THROW("failed to read normal: unhandled mapping mode")
		}

		tempNormal = kFinalGlobalTransform.MultT(tempNormal);
		normal = {	static_cast<float>(tempNormal[0u]),
					static_cast<float>(tempNormal[1u] ),
					static_cast<float>(tempNormal[2u]) };
	}

	void LoaderFbx::readTangent(FbxMesh* mesh, math::Vec3Holder<float>& tangent, uint32_t ctrlPointIndex, uint32_t vertexCounter, const FbxAMatrix& kFinalGlobalTransform)
	{
		if (mesh->GetElementTangentCount() < 1)
			THROW("failed to read tangent: invalid tangent count")

		FbxGeometryElementTangent* vertexTangent { mesh->GetElementTangent(0) };
		if (!vertexTangent)
			THROW("failed to read tangent: invalid FbxGeometryElementTangent")

		FbxGeometryElementTangent::EMappingMode mappingMode { vertexTangent->GetMappingMode() };
		FbxGeometryElementTangent::EReferenceMode refMode { vertexTangent->GetReferenceMode() };

		FbxVector4 tempTangent {};
		switch (mappingMode)
		{
			case FbxGeometryElementTangent::EMappingMode::eByControlPoint:
			{
				switch (refMode)
				{
					case FbxGeometryElementTangent::EReferenceMode::eDirect:
					{
						tempTangent = vertexTangent->GetDirectArray().GetAt(ctrlPointIndex);
					}
					break;

					case FbxGeometryElementTangent::EReferenceMode::eIndexToDirect:
					{
						uint32_t index { static_cast<uint32_t>(vertexTangent->GetIndexArray().GetAt(ctrlPointIndex)) };
						tempTangent = vertexTangent->GetDirectArray().GetAt(index);
					}
					break;

					default:
					THROW("failed to read tangent: unhandled reference mode")
				}
			}
			break;

			case FbxGeometryElementTangent::EMappingMode::eByPolygonVertex:
			{
				switch (vertexTangent->GetReferenceMode())
				{
					case FbxGeometryElementTangent::EReferenceMode::eDirect:
					{
						tempTangent = vertexTangent->GetDirectArray().GetAt(vertexCounter);
					}
					break;

					case FbxGeometryElementTangent::EReferenceMode::eIndexToDirect:
					{
						uint32_t index { static_cast<uint32_t>(vertexTangent->GetIndexArray().GetAt(vertexCounter)) };
						tempTangent = vertexTangent->GetDirectArray().GetAt(index);
					}
					break;

					default:
					THROW("failed to read tangent: unhandled reference mode")
				}
			}
			break;
			
			default:
			THROW("failed to read tangent: unhandled mapping mode")
		}

		tempTangent = kFinalGlobalTransform.MultT(tempTangent);
		double handedness { tempTangent[3] };
		tangent = { static_cast<float>(tempTangent[0] * handedness),
					static_cast<float>(tempTangent[1] * handedness),
					static_cast<float>(tempTangent[2] * handedness) };
	}

	// Parsing skeleton

	void LoaderFbx::processSkeletonHierarchy(FbxNode* node, anim::Skeleton* skull)
	{
		if (!skull || !node)
			return;

		FbxNodeAttribute* attribute { node->GetNodeAttribute() };
		if (attribute && attribute->GetAttributeType() == FbxNodeAttribute::eSkeleton)
			return;
	
		processSkeletonHierarchyRecursively(node->GetChild(0), skull);
	}

	uint32_t LoaderFbx::processSkeletonHierarchyRecursively(FbxNode* node, anim::Skeleton* skull)
	{
		int32_t nbChild { node->GetChildCount() };
		int32_t currentIndex { static_cast<int32_t>(skull->jointCount()) };

		skull->addJoint({ node->GetName(), static_cast<uint32_t>(nbChild) });

		for (int32_t i { 0u }; i < nbChild; i++)
			skull->joint(currentIndex).mChildrenIndex[i] = processSkeletonHierarchyRecursively(node->GetChild(i), skull);
	
		return currentIndex;
	}

	// Parsing animations

	void LoaderFbx::processJointsAndAnimations(FbxNode* node, anim::Skeleton* skull, std::vector<anim::Animation>& animations)
	{
		FbxMesh* currMesh { node->GetMesh() };
		uint32_t numOfDeformers { static_cast<uint32_t>(currMesh->GetDeformerCount()) };

		FbxVector4 translation { node->GetGeometricTranslation(FbxNode::eSourcePivot) };
		FbxVector4 rotation { node->GetGeometricRotation(FbxNode::eSourcePivot) };
		FbxVector4 scaling { node->GetGeometricScaling(FbxNode::eSourcePivot) };

		FbxAMatrix geometryTransform { translation, rotation, scaling };

		for (uint32_t deformerIndex{ 0u }; deformerIndex < numOfDeformers; ++deformerIndex)
		{
			FbxSkin* currSkin{ reinterpret_cast<FbxSkin*>(currMesh->GetDeformer(deformerIndex, FbxDeformer::eSkin)) };
			if (!currSkin)
				continue;
			
			uint32_t stackCount { static_cast<uint32_t>(mScene->GetSrcObjectCount<FbxAnimStack>()) };
			for (uint32_t animIndex{0u}; animIndex < stackCount; animIndex++)
			{
				FbxAnimStack* currAnimStack { mScene->GetSrcObject<FbxAnimStack>(animIndex) };
				FbxString animStackName { currAnimStack->GetName() };

				std::string animName { animStackName.Buffer() };
				std::vector<anim::Keyframe> keyFrames {};
				
				FbxTakeInfo* takeInfo { mScene->GetTakeInfo(animStackName) };
				FbxTime start { takeInfo->mLocalTimeSpan.GetStart() };
				FbxTime end { takeInfo->mLocalTimeSpan.GetStop() };

				const FbxLongLong kAnimationLength { end.GetFrameCount(FbxTime::eFrames24) - start.GetFrameCount(FbxTime::eFrames24) + 1 };
				keyFrames.resize(static_cast<const uint32_t>(kAnimationLength));
			
				uint32_t numOfClusters { static_cast<uint32_t>(currSkin->GetClusterCount()) };
				for (uint32_t clusterIndex{ 0u }; clusterIndex < numOfClusters; ++clusterIndex)
				{
					FbxCluster* currCluster{ currSkin->GetCluster(clusterIndex) };
					const char* kCurrJointName{ currCluster->GetLink()->GetName() };
					uint32_t currJointIndex{ skull->jointIndexFromName(kCurrJointName) };

					FbxAMatrix linkMatrix;
					currCluster->GetTransformLinkMatrix(linkMatrix);

					FbxAMatrix globalBindInverse { linkMatrix.Inverse() * geometryTransform };

					math::Quatf rot {};
					math::Vec3Holder<float> scale {}, trans {};
					fromFbxMatrix(globalBindInverse, trans, rot, scale);

					skull->joint(currJointIndex).mGlobalBind = math::Transf(trans, rot, scale);

					uint32_t numOfIndices { static_cast<uint32_t>(currCluster->GetControlPointIndicesCount()) };
					for (uint32_t i{ 0u }; i < numOfIndices; ++i)
					{
						anim::VertexBlendingInfo blendingPair { static_cast<float>(currJointIndex), static_cast<float>(currCluster->GetControlPointWeights()[i]) };
						uint32_t id { static_cast<uint32_t>(currCluster->GetControlPointIndices()[i]) };

						mControlPoints[id].mBlendingInfo.emplace_back(blendingPair);
					}

					for (FbxLongLong i { start.GetFrameCount(FbxTime::eFrames24) }; i <= end.GetFrameCount(FbxTime::eFrames24); ++i)
					{
						anim::Keyframe& currKey { keyFrames[static_cast<uint32_t>(i)] };
						FbxTime currTime {};
						currTime.SetFrame(i, FbxTime::eFrames24);

						FbxNode* link { currCluster->GetLink() };
						FbxAMatrix matrix { link->EvaluateGlobalTransform(currTime) };

						fromFbxMatrix(matrix, trans, rot, scale);
						currKey.addTransform({ trans, rot, scale });
					}
				}

				animations.emplace_back(animName, std::move(keyFrames));
			} 
		}

		anim::VertexBlendingInfo missingBlendingIndexWeightPair {};
		uint32_t controlPointCount { static_cast<uint32_t>(mControlPoints.size()) };
		for (uint32_t i { 0u } ; i < controlPointCount ; ++i)
		{
			for (uint32_t j { static_cast<uint32_t>(mControlPoints[i].mBlendingInfo.size()) } ; j < anim::ControlPoint::skMaxBlendingInfo ; ++j)
				mControlPoints[i].mBlendingInfo.emplace_back(missingBlendingIndexWeightPair);
		}
	}
	
	// Helpers

	void LoaderFbx::optimize(std::vector<anim::Vertex>& tempVertices, std::vector<anim::Vertex>& vertices, std::vector<uint32_t>& indices)
	{
		std::unordered_map<anim::Vertex, uint32_t> hashMapVertices;
		uint32_t size{ static_cast<uint32_t>(mTriangles.size()) };

		for (uint32_t i{ 0u }; i < size; i++)
		{
			if (hashMapVertices.count(tempVertices[i]) == 0u)
			{
				hashMapVertices[tempVertices[i]] = static_cast<uint32_t>(vertices.size());
				vertices.emplace_back(tempVertices[i]);
			}

			indices.emplace_back(hashMapVertices[tempVertices[i]]);
		}

		tempVertices.clear();
	}

	FbxAMatrix LoaderFbx::globalMatrix(FbxNode* node) const
	{
		if (!node)
			return {};

		FbxVector4 translation { node->GetGeometricTranslation(FbxNode::eSourcePivot) };
		FbxVector4 rotation { node->GetGeometricRotation(FbxNode::eSourcePivot) };
		FbxVector4 scaling { node->GetGeometricScaling(FbxNode::eSourcePivot) };

		FbxAMatrix geometryTransform { translation, rotation, scaling };
		if (!mScene)
			return geometryTransform;

		FbxAMatrix globalTransform { mScene->GetAnimationEvaluator()->GetNodeGlobalTransform(node) };
		return globalTransform * geometryTransform;
	}

	math::Mat4f LoaderFbx::fromFbxMatrix(const FbxAMatrix& kMat) const
	{
		math::Mat4f tempMat {};
		for (uint8_t i { 0u }; i < 4u; i++)
		{
			for (uint8_t j { 0u }; j < 4u; j++)
				tempMat.at(i, j) = static_cast<float>(kMat.Get(i, j));
		}

		return tempMat;
	}

	void LoaderFbx::fromFbxMatrix(const FbxAMatrix& kMat, math::Vec3Holder<float>& position, math::Quatf& rotation, math::Vec3Holder<float>& scale) const
	{
		FbxVector4 pos { kMat.GetT() };
		position.mX = static_cast<float>(pos[0]);
		position.mY = static_cast<float>(pos[1]);
		position.mZ = static_cast<float>(pos[2]);

		FbxQuaternion rot { kMat.GetQ() };
		rotation.mX = static_cast<float>(rot[0]);
		rotation.mY = static_cast<float>(rot[1]);
		rotation.mZ = static_cast<float>(rot[2]);
		rotation.mW = static_cast<float>(rot[3]);

		FbxVector4 sca { kMat.GetS() };
		scale.mX = static_cast<float>(sca[0]);
		scale.mY = static_cast<float>(sca[1]);
		scale.mZ = static_cast<float>(sca[2]);
	}
}