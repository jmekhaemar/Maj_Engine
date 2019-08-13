#ifndef _ASSET_H_
#define _ASSET_H_

#include <experimental/filesystem>

#include <Service.h>
#include <ISerializable.h>
#include <Event.h>

namespace maj
{
	template<typename T>
	class Asset final : public ISerializable
	{
		ENABLE_META(Asset<T>)

	public:
		class Handler final : public Service
		{
		public:
			Handler();
			~Handler() = default;

			void initialize(const uint32_t kGuid, const std::experimental::filesystem::path& kPath);

			template<typename... Args>
			T* add(const std::string& kName, Args&&... args);
			T* add(const uint32_t kGuid);

			bool exists(const std::string& kName);
			T* get(const std::string& kName);
			std::string name(uint32_t guid) const;

			void erase(const uint32_t kGuid);
			void clean();
			void clear(bool save);

		private:
			std::unordered_map<uint32_t, std::pair<std::experimental::filesystem::path, uint32_t>> mAssetsIds;
			std::unordered_map<std::string, T> mAssets;
		};

	public:
		typedef T AssetType;

		static void initialize(const uint32_t kGuid, const std::experimental::filesystem::path& kPath);

		template<typename... Args>
		static T* add(const std::string& kName, Args&&... args);
		static T* add(const uint32_t kGuid);
		
		static bool exists(const std::string& kName);
		static T* get(const std::string& kName);
		static std::string name(uint32_t guid);

		static void erase(const uint32_t kGuid);
		static void clean();
		static void clear(bool save);

	private:
		static std::string assetFromPath(const std::experimental::filesystem::path& kPath);

	public:
		LOAD_CONSTRUCTOR_CUSTOM(Asset<T>)

		Asset();
		Asset(const T* kAsset, bool isSerialized = true);
		Asset(const Asset<T>& kOther);
		~Asset();

		void onChange(std::function<void()> callback);
		// /!\ use only if you now what you're doing, created for editor purpose
		void change() const; 

		bool empty() const;
		const T* get() const;

		const T& operator*() const;
		const T* operator->() const;

		Asset& operator=(const Asset<T>& kOther);
		Asset& operator=(const T* kAsset);
		Asset& operator=(const uint32_t guid);

		bool operator==(const T* kAsset) const;
		bool operator!=(const T* kAsset) const;

	private:
		T* mAsset;
		bool mIsSerialized;

		std::function<void()> mOnChange;
		uint32_t mOnChangeId;
	};
}

#include "Asset.inl"

#endif