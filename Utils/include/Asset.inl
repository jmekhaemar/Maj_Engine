#ifndef _ASSET_INL_
#define _ASSET_INL_

#include "Asset.h"

namespace maj
{
	template<typename T>
	DEFINE_HASH_OVERLOAD(Asset<T>)

	template <typename T>
	Asset<T>::Handler::Handler()
		: Service { typeid(Handler).name() }, mAssetsIds {}, mAssets {}
	{}

	template <typename T>
	void Asset<T>::Handler::initialize(const uint32_t kGuid, const std::experimental::filesystem::path& kPath)
	{
		mAssetsIds.try_emplace(kGuid, kPath, 0u);
	}

	template <typename T>
	template <typename... Args>
	T* Asset<T>::Handler::add(const std::string& kName, Args&&... args)
	{
		if (mAssets.count(kName) == 0u)
		{
			mAssets.try_emplace(kName, std::forward<Args>(args)...);
			initialize(mAssets.at(kName).mGuid, std::experimental::filesystem::path { kName + ".asset" });
		}

		return &mAssets.at(kName);
	}

	template <typename T>
	T* Asset<T>::Handler::add(const uint32_t kGuid)
	{
		if (mAssetsIds.count(kGuid) == 0u)
		{
			LOG(LogWarning, "failed to load asset: asset id \'" << kGuid << "\' not found")
			return nullptr;
		}

		mAssetsIds.at(kGuid).second += 1u;
		const std::experimental::filesystem::path& kPath { mAssetsIds.at(kGuid).first };
		const std::string kAssetName { assetFromPath(kPath) };
		if (mAssets.count(kAssetName) == 0u)
		{
			LOG(LogInfo, "loading asset \'" << kAssetName << "\'")

			std::unique_ptr<JsonNode> root {};
			try
			{
				root = JsonNode::parse(kPath.generic_string().c_str());
				mAssets.try_emplace(kAssetName, root.get());
			}
			catch (const std::exception& e)
			{
				LOG(LogWarning, "failed to load asset \'" << kAssetName << "\':" << e.what())
				return nullptr;
			}
		}

		return &mAssets.at(kAssetName);
	}

	template <typename T>
	bool Asset<T>::Handler::exists(const std::string& kName)
	{
		return mAssets.count(kName) != 0u;
	}

	template <typename T>
	T* Asset<T>::Handler::get(const std::string& kName)
	{
		if (mAssets.count(kName) == 0u)
			THROW("failed to get asset: asset \'" + kName + "\' not found")

		return &mAssets.at(kName);
	}

	template<typename T>
	std::string Asset<T>::Handler::name(uint32_t guid) const
	{
		if (mAssetsIds.count(guid) == 0u)
		{
			LOG(LogWarning, "failed to get asset: asset \'" << std::to_string(guid) << "\' not found")
			return "";
		}

		return assetFromPath(mAssetsIds.at(guid).first);
	}

	template <typename T>
	void Asset<T>::Handler::erase(const uint32_t kGuid)
	{
		if (mAssetsIds.count(kGuid) == 0u || mAssetsIds.at(kGuid).second == 0u)
			return;

		mAssetsIds.at(kGuid).second -= 1u;
	}

	template <typename T>
	void Asset<T>::Handler::clean()
	{
		for (std::unordered_map<uint32_t, std::pair<std::experimental::filesystem::path, uint32_t>>::const_iterator it { mAssetsIds.begin() } ; it != mAssetsIds.end() ; ++it)
		{
			const std::string kAssetName { assetFromPath(it->second.first) };
			if (it->second.second == 0u && mAssets.count(kAssetName) != 0u)
			{
				LOG(LogInfo, "cleaning asset \'" << kAssetName << "\'")
				mAssets.erase(kAssetName);
			}
		}
	}

	template <typename T>
	void Asset<T>::Handler::clear(bool save)
	{
		for (const std::pair<uint32_t, std::pair<std::experimental::filesystem::path, uint32_t>>& pair : mAssetsIds)
		{
			const std::experimental::filesystem::path& kPath { pair.second.first };
			const std::string kAssetName { assetFromPath(kPath) };
			
			mAssets.erase(kAssetName);
		}

		mAssets.clear();
		mAssetsIds.clear();
	}
	
	template <typename T>
	void Asset<T>::initialize(const uint32_t kGuid, const std::experimental::filesystem::path& kPath)
	{
		Service::get<Handler>()->initialize(kGuid, kPath);
	}

	template <typename T>
	template <typename... Args>
	T* Asset<T>::add(const std::string& kName, Args&&... args)
	{
		return Service::get<Handler>()->add(kName, std::forward<Args>(args)...);
	}

	template <typename T>
	T* Asset<T>::add(const uint32_t kGuid)
	{
		return Service::get<Handler>()->add(kGuid);
	}

	template <typename T>
	bool Asset<T>::exists(const std::string& kName)
	{
		return Service::get<Handler>()->exists(kName);
	}

	template <typename T>
	T* Asset<T>::get(const std::string& kName)
	{
		return Service::get<Handler>()->get(kName);
	}

	template <typename T>
	std::string Asset<T>::name(uint32_t guid)
	{
		return Service::get<Handler>()->name(guid);
	}

	template <typename T>
	void Asset<T>::erase(const uint32_t kGuid)
	{
		Service::get<Handler>()->erase(kGuid);
	}

	template <typename T>
	void Asset<T>::clean()
	{
		Service::get<Handler>()->clean();
	}

	template <typename T>
	void Asset<T>::clear(bool save)
	{
		Service::get<Handler>()->clear(save);
	}

	template <typename T>
	std::string Asset<T>::assetFromPath(const std::experimental::filesystem::path& kPath)
	{
		const std::string kStem { kPath.stem().generic_string() };
		if (!kPath.has_parent_path())
			return kStem;

		const std::experimental::filesystem::path kParentPath { kPath.parent_path() };
		const std::string kParentStr { kParentPath.generic_string() };

		if (kParentStr == ".")
			return kStem;

		return assetFromPath(kParentPath) + "/" + kStem;
	}

	template<typename T>
	Asset<T>::Asset()
		: mAsset { nullptr }, mIsSerialized { true }, mOnChangeId{ 0u }
	{}

	template<typename T>
	Asset<T>::Asset(JsonNode* root)
		: mAsset { nullptr }, mIsSerialized { true }, mOnChangeId{ 0u }
	{
		load(root);
		initialize();
	}

	template<typename T>
	Asset<T>::Asset(const T* kAsset, bool isSerialized)
		: mAsset{ const_cast<T*>(kAsset) }, mIsSerialized { isSerialized }, mOnChangeId{ 0u }
	{
		if (!mAsset)
		{ 
			LOG(LogWarning, "failed to create asset: no asset")
			return;
		}

		mGuid = kAsset->mGuid;

		if (mIsSerialized)
			add(mGuid);
	}

	template<typename T>
	Asset<T>::Asset(const Asset<T>& kOther)
		: ISerializable{ kOther }, mAsset{ kOther.mAsset }, mIsSerialized{ kOther.mIsSerialized }, mOnChangeId{ 0u }
	{
		if (mIsSerialized)
			add(mGuid);
	}

	template<typename T>
	Asset<T>::~Asset()
	{
		if(mAsset)
			mAsset->mOnChange -= mOnChangeId;
		erase(mGuid);
	}

	template<typename T>
	void Asset<T>::onChange(std::function<void()> callback)
	{
		if (mAsset)
		{
			if(mOnChange)
				mAsset->mOnChange -= mOnChangeId;

			mOnChangeId = mAsset->mOnChange + callback;
		}

		mOnChange = callback;
	}

	template<typename T>
	void Asset<T>::change() const
	{
		if (mOnChange)
			mOnChange();
	}

	template<typename T>
	void Asset<T>::initialize()
	{
		mAsset = add(mGuid);
	}

	template<typename T>
	bool Asset<T>::empty() const
	{
		return mAsset == nullptr;
	}

	template <typename T>
	const T* Asset<T>::get() const
	{
		return mAsset;
	}

	template<typename T>
	const T& Asset<T>::operator*() const
	{
		return *mAsset;
	}

	template<typename T>
	const T* Asset<T>::operator->() const
	{
		return mAsset;
	}

	template<typename T>
	Asset<T>& Asset<T>::operator=(const Asset<T>& kOther)
	{
		if(mAsset)
			mAsset->mOnChange -= mOnChangeId;

		erase(mGuid);

		mAsset = kOther.mAsset;
		mGuid = kOther.mGuid;
		mIsSerialized = kOther.mIsSerialized;

		if (mAsset && mOnChange)
			mOnChangeId = mAsset->mOnChange + mOnChange;

		if (mIsSerialized)
			add(mGuid);

		return *this;
	}

	template<typename T>
	Asset<T>& Asset<T>::operator=(const T* kAsset)
	{
		if (mAsset)
			mAsset->mOnChange -= mOnChangeId;
		erase(mGuid);

		mAsset = const_cast<T*>(kAsset);
		if (!mAsset)
			THROW("failed to assign asset: no asset")

		mGuid = kAsset->mGuid;
		mIsSerialized = true;

		if (mOnChange)
			mOnChangeId = mAsset->mOnChange + mOnChange;

		add(mGuid);

		return *this;
	}

	template<typename T>
	Asset<T>& Asset<T>::operator=(const uint32_t guid)
	{
		if (mAsset)
			mAsset->mOnChange -= mOnChangeId;
		erase(mGuid);

		mAsset = add(guid);
		if (!mAsset)
			THROW("failed to assign asset: no asset")

		mGuid = guid;
		mIsSerialized = true;

		if (mOnChange)
			mOnChangeId = mAsset->mOnChange + mOnChange;

		return *this;
	}

	template <typename T>
	bool Asset<T>::operator==(const T* kAsset) const
	{
		return mAsset == kAsset;
	}

	template <typename T>
	bool Asset<T>::operator!=(const T* kAsset) const
	{
		return mAsset != kAsset;
	}
}

#endif