#ifndef _ISERIALIZABLE_H_
#define _ISERIALIZABLE_H_

#include <functional>

#include "MetaMacros.h"
#include "JsonNode.h"

namespace maj
{
	class ISerializable
	{
	public:
		inline static std::unordered_map<uint32_t, std::function<ISerializable*(JsonNode*)>>& registeredClass();
		static void registerClass(const std::unordered_map<uint32_t, std::function<ISerializable*(JsonNode*)>>& kClasses);

		template<typename T>
		struct Save final
		{
			template<uint32_t i>
			static void invoke(T* obj, JsonNode* root);
		};

		template<typename T>
		struct Load final
		{
			template<uint32_t i>
			static void invoke(T* obj, JsonNode* root);
		};

		ISerializable();
		ISerializable(const ISerializable& kOther);

		virtual uint32_t hash() = 0;

		virtual void save(const char*) = 0;
		virtual void save(JsonNode*) = 0;

		virtual void load(JsonNode*) = 0;
		virtual void initialize();

		template<typename T>
		void serialize(JsonNode* root);

		template<typename T>
		void deserialize(JsonNode* root);

		uint32_t mGuid;
	};

	namespace meta
	{
		template<typename T>
		struct Register final
		{
			Register();
		};
	}
}

#include "ISerializable.inl"

#endif