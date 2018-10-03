#ifndef _REGISTERED_INSPECTOR_H_
#define _REGISTERED_INSPECTOR_H_

#include "inspector/WInspectorAsset.h"

#include "inspector/custom/WVec2uiInspector.h"
#include "inspector/custom/WVec3fInspector.h"
#include "inspector/custom/WVec4fInspector.h"
#include "inspector/custom/WQuatfInspector.h"
#include "inspector/custom/WIComponentInspector.h"
#include "inspector/custom/WICColliderInspector.h"
#include "inspector/custom/WSphereColliderInspector.h"
#include "inspector/custom/WCBoxColliderInspector.h"
#include "inspector/custom/WCCapsuleColliderInspector.h"
#include "inspector/custom/WCCameraInspector.h"
#include "inspector/custom/WAssetInspector.h"
#include "inspector/custom/WCTransformInspector.h"
#include "inspector/custom/WActorInspector.h"
#include "inspector/custom/WShaderInspector.h"
#include "inspector/custom/WColorInspector.h"
#include "inspector/custom/WIElementInspector.h"
#include "inspector/custom/WCWidgetInspector.h"

#include <scene/CTransform.h>
#include <scene/CCamera.h>
#include <scene/CDirectionalLight.h>
#include <scene/CPointLight.h>
#include <scene/CSkeletalMesh.h>
#include <scene/CStaticMesh.h>
#include <scene/ASkybox.h>
#include <scene/SceneHandler.h>

#include <Vec2.h>
#include <Vec3.h>
#include <Vec4.h>
#include <Quaternion.h>
#include <Transform.h>
#include <Color.h>

#include <scene/CScript.h>

#include <physic/CBoxCollider.h>
#include <physic/CCapsuleCollider.h>
#include <physic/CSphereCollider.h>
#include <physic/Layer.h>

#include <gui/CWidget.h>
#include <gui/Sprite.h>
#include <gui/Text.h>
#include <gui/Font.h>

#include <MaterialLayout.h>
#include <Material.h>
#include <physic/PhysicMaterial.h>
#include <Image.h>
#include <Texture.h>
#include <resources/Model.h>
#include <resources/SkeletalModel.h>

namespace maj
{
	typedef Asset<gui::Font> AssetFont;
	typedef Asset<rhi::MaterialLayout> AssetMaterialLayout;
	typedef Asset<rhi::Image> AssetImage;
	typedef Asset<physic::PhysicMaterial> AssetPhysicMaterial;
	typedef Asset<rhi::Material> AssetMaterial;
	typedef Asset<rhi::Texture> AssetTexture;
	typedef Asset<resources::Model> AssetModel;
	typedef Asset<resources::SkeletalModel> AssetSkeletalModel;

	typedef Engine::Config config;

	REGISTER_INSPECTOR_CUSTOM(AssetFont, editor::WAssetInspector)
	REGISTER_INSPECTOR_CUSTOM(AssetMaterialLayout, editor::WAssetInspector)
	REGISTER_INSPECTOR_CUSTOM(AssetImage, editor::WAssetInspector)
	REGISTER_INSPECTOR_CUSTOM(AssetPhysicMaterial, editor::WAssetInspector)
	REGISTER_INSPECTOR_CUSTOM(AssetMaterial, editor::WAssetInspector)
	REGISTER_INSPECTOR_CUSTOM(AssetTexture, editor::WAssetInspector)
	REGISTER_INSPECTOR_CUSTOM(AssetModel, editor::WAssetInspector)
	REGISTER_INSPECTOR_CUSTOM(AssetSkeletalModel, editor::WAssetInspector)
	REGISTER_INSPECTOR(config)
	REGISTER_INSPECTOR_CUSTOM(Color, editor::WColorInspector)
}

namespace maj::ww
{

	typedef Viewport::Info info;
	REGISTER_INSPECTOR(info)
}

namespace maj::rhi
{
	REGISTER_ASSET_INSPECTOR(Texture)
	REGISTER_INSPECTOR_CUSTOM(Shader, editor::WShaderInspector)
	REGISTER_INSPECTOR(VertexBinding)
	REGISTER_INSPECTOR(VertexAttribute)
	REGISTER_INSPECTOR(PipelineState)
	REGISTER_INSPECTOR(LayoutBinding)
	REGISTER_INSPECTOR(LayoutSet)
	REGISTER_ASSET_INSPECTOR(Material)
	REGISTER_INSPECTOR(DepthBias)
	REGISTER_INSPECTOR(SampleState)
	REGISTER_INSPECTOR(DepthBound)
	REGISTER_INSPECTOR(StencilState)
	REGISTER_INSPECTOR(BlendState)
	REGISTER_INSPECTOR(BlendConstant)
	typedef Material::Info MaterialInfo;
	REGISTER_INSPECTOR(MaterialInfo)
	REGISTER_ASSET_INSPECTOR(Image)
	typedef Image::ImgInfo ImageInfo;
	REGISTER_INSPECTOR(ImageInfo)
	REGISTER_INSPECTOR(MaterialBinding)
	REGISTER_INSPECTOR(MaterialInstance)
	REGISTER_ASSET_INSPECTOR(MaterialLayout)
	REGISTER_INSPECTOR(ExtensionList)
}

namespace maj::resources
{
	REGISTER_ASSET_INSPECTOR(Model)
	REGISTER_ASSET_INSPECTOR(SkeletalModel)
}

namespace maj::scene
{
	REGISTER_INSPECTOR(CScript)
	REGISTER_INSPECTOR(ASkybox)
	REGISTER_INSPECTOR_CUSTOM(Actor, editor::WActorInspector)
	REGISTER_INSPECTOR_CUSTOM(CTransform, editor::WCTransformInspector)
	REGISTER_COMPONENT_INSPECTOR_CUSTOM(CCamera, editor::WCCameraInspector)
	REGISTER_COMPONENT_INSPECTOR(CDirectionalLight)
	REGISTER_COMPONENT_INSPECTOR(CPointLight)
	REGISTER_INSPECTOR(ICLight)
	REGISTER_COMPONENT_INSPECTOR(CSkeletalMesh)
	REGISTER_COMPONENT_INSPECTOR(CStaticMesh)
	REGISTER_INSPECTOR(ICMesh)
	REGISTER_INSPECTOR_CUSTOM(IComponent, editor::WIComponentInspector)
	typedef SceneHandler::SceneConfig SceneConf;
	REGISTER_INSPECTOR(SceneConf)
}

namespace maj::math
{
	REGISTER_INSPECTOR(Vec2f)
	REGISTER_INSPECTOR_CUSTOM(Vec2ui, editor::WVec2uiInspector)
	REGISTER_INSPECTOR_CUSTOM(Vec3f, editor::WVec3fInspector)
	REGISTER_INSPECTOR_CUSTOM(Vec4f, editor::WVec4fInspector)
	REGISTER_INSPECTOR_CUSTOM(Quatf, editor::WQuatfInspector)
	REGISTER_INSPECTOR(Transf)
}

namespace maj::physic
{
	REGISTER_COMPONENT_INSPECTOR_CUSTOM(CBoxCollider, editor::WCBoxColliderInspector)
	REGISTER_COMPONENT_INSPECTOR_CUSTOM(CCapsuleCollider, editor::WCCapsuleColliderInspector)
	REGISTER_COMPONENT_INSPECTOR_CUSTOM(CSphereCollider, editor::WSphereColliderInspector)
	REGISTER_INSPECTOR_CUSTOM(ICCollider, editor::WICColliderInspector)
	REGISTER_ASSET_INSPECTOR(PhysicMaterial)
	typedef physic::LayerFilter filter;
	REGISTER_INSPECTOR(filter)
}

namespace maj::gui
{
	typedef Font::FontInfo FontInfo;
	//REGISTER_INSPECTOR(FontInfo)
	REGISTER_ASSET_INSPECTOR(Font)
	REGISTER_COMPONENT_INSPECTOR_CUSTOM(CWidget, editor::WCWidgetInspector)
	REGISTER_INSPECTOR_CUSTOM(IElement, editor::WIElementInspector)
	REGISTER_INSPECTOR(Sprite)
	REGISTER_INSPECTOR(Text)
}

#endif