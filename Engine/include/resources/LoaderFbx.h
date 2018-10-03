#ifndef _LOADER_FBX_H_
#define _LOADER_FBX_H_

#include <fbxsdk.h>

#include <Service.h>

#include "anim/Skeleton.h"
#include "anim/AnimVertex.h"
#include "anim/ControlPoint.h"
#include "anim/Animation.h"

#define MAX_UV_SET 1u

namespace maj::resources
{
	class LoaderFbx final : public Service
	{
	public:
		LoaderFbx();
		~LoaderFbx();

		void load(const char* kPath, std::vector<anim::Vertex>& vertices, std::vector<uint32_t>& indices, anim::Skeleton* skull,
				 std::vector<anim::Animation>& animations);

	private:
		// Loading/Parsing nodes
		void initFbx(const char* kPath);
		void parseFbx(const char* kPath, std::vector<anim::Vertex>& vertices, std::vector<uint32_t>& indices, anim::Skeleton* skull,
					 std::vector<anim::Animation>& animations);

		void processNode(FbxNode* node, std::vector<anim::Vertex>& vertices, std::vector<uint32_t>& indices, anim::Skeleton* skull, std::vector<anim::Animation>& animations);

		void addMesh(FbxNode* node, std::vector<anim::Vertex>& vertices);

		// Parsing attributes
		void processControlPoints(FbxNode* node);

		void readUv(FbxMesh* mesh, math::Vec2Holder<float>& uv, uint32_t indexI, uint32_t indexJ, uint32_t uvLayer);
		void readNormal(FbxMesh* mesh, math::Vec3Holder<float>& normal, uint32_t ctrlPointIndex, uint32_t vertexCounter, const FbxAMatrix& kFinalGlobalTransform);
		void readTangent(FbxMesh* mesh, math::Vec3Holder<float>& tangent, uint32_t ctrlPointIndex, uint32_t vertexCounter, const FbxAMatrix& kFinalGlobalTransform);

		// Parsing skeleton
		void processSkeletonHierarchy(FbxNode* node, anim::Skeleton* skull);
		uint32_t processSkeletonHierarchyRecursively(FbxNode* node, anim::Skeleton* skull);

		// Parsing animations
		void processJointsAndAnimations(FbxNode* node, anim::Skeleton* skull,std::vector<anim::Animation>& animations);
		
		// Helpers
		void optimize(std::vector<anim::Vertex>& tempVertices, std::vector<anim::Vertex>& vertices, std::vector<uint32_t>& indices);

		FbxAMatrix globalMatrix(FbxNode* node) const;
		math::Mat4f fromFbxMatrix(const FbxAMatrix& kMat) const;
		void fromFbxMatrix(const FbxAMatrix& kMat, math::Vec3Holder<float>& position, math::Quatf& rotation, math::Vec3Holder<float>& scale) const;

		FbxScene* mScene;
		FbxManager* mManager;
		bool mHasAnimation;

		std::vector<anim::ControlPoint> mControlPoints;
		std::vector<uint32_t> mTriangles;
	};
}

#endif