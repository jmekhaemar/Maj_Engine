#ifndef _ASSETS_HANDLER_H_
#define _ASSETS_HANDLER_H_

#include <Service.h>

#include <common/LoaderImg.h>

#include "resources/LoaderFont.h"
#include "resources/LoaderFbx.h"
#include "resources/LoaderObj.h"

#include <Texture.h>
#include <Material.h>
#include <Image.h>
#include <MaterialLayout.h>

#include "resources/Model.h"
#include "resources/SkeletalModel.h"
#include "gui/Font.h"
#include "physic/PhysicMaterial.h"

namespace maj::resources
{
	class AssetsHandler : public Service
	{
	public:
		AssetsHandler();
		~AssetsHandler();

		void load(const char* kRootPath);
		void clean();

	private:
		std::pair<uint32_t, uint32_t> read(const std::experimental::filesystem::path& kPath);

		Asset<SkeletalModel>::Handler mSkeletalModelHandler;
		Asset<Model>::Handler mModelHandler;
		Asset<rhi::Image>::Handler mImgHandler;
		Asset<rhi::Texture>::Handler mTextureHandler;
		Asset<rhi::MaterialLayout>::Handler msMatLayoutHandler;
		Asset<rhi::Material>::Handler mMatHandler;
		Asset<physic::PhysicMaterial>::Handler mPhysicMatHandler;
		Asset<gui::Font>::Handler mFontHandler;

		rhi::LoaderImg mLoaderImg;
		LoaderFont mLoaderFont;
		LoaderFbx mLoaderFbx;
		LoaderObj mLoaderObj;
	};
}

#endif