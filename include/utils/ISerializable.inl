#ifndef _ISERIALIZABLE_INL_
#define _ISERIALIZABLE_INL_

#include "ISerializable.h"
#include <Logging.h>

namespace maj
{
	std::unordered_map<uint32_t, std::function<ISerializable*(JsonNode*)>>& ISerializable::registeredClass()
	{
		static std::unordered_map<uint32_t, std::function<ISerializable*(JsonNode*)>> sRegistered {};
		return sRegistered;
	}

	template <typename T>
	template <uint32_t i>
	void ISerializable::Save<T>::invoke(T* obj, JsonNode* root)
	{
		typedef meta::Field<T, i> CurrentField;

		if constexpr (CurrentField::isArray())
		{
			root->addNode(CurrentField::name(), new JsonNode { JsonNode::EType::ARRAY });
			for (auto iterator = CurrentField::get(obj).begin(); iterator != CurrentField::get(obj).end(); iterator++) // TODO auto
			{
				JsonNode* arrayObj { new JsonNode { JsonNode::EType::OBJECT } };
				typedef typename CurrentField::Type::value_type Value;

				if constexpr (std::is_pointer<Value>::value)
				{
					if ((*iterator) != nullptr)
						(*iterator)->save(arrayObj);
				}
				else
					iterator->save(arrayObj);

				(*root)[CurrentField::name()]->addChild(arrayObj);
			}
		}
		else if constexpr (CurrentField::isMap())
		{
			JsonNode* map { new JsonNode { JsonNode::EType::ARRAY } };
			root->addNode(CurrentField::name(), map);
			for (auto iterator = CurrentField::get(obj).begin(); iterator != CurrentField::get(obj).end(); iterator++) // TODO auto
			{
				JsonNode* mapObj { new JsonNode { JsonNode::EType::OBJECT } };
				typedef typename CurrentField::Type::mapped_type Value;
				typedef typename CurrentField::Type::key_type Key;

				if constexpr (!meta::Class< Key >::value)
					mapObj->addNode("key", new JsonNode{ static_cast<int32_t>(iterator->first) });
				else
				{
					JsonNode* keyObj { new JsonNode { JsonNode::EType::OBJECT } };
					static_cast<Key>(iterator->first).save(keyObj);
					mapObj->addNode("key", keyObj);
				}

				JsonNode* valueObj{ nullptr };
				if constexpr (std::is_pointer< Value >::value)
				{
					valueObj = new JsonNode{ JsonNode::EType::OBJECT };
					if ((iterator->second) != nullptr)
						iterator->second->save(valueObj);
				}
				else if constexpr(meta::Class< Value >::value)
				{
					valueObj = new JsonNode{ JsonNode::EType::OBJECT };
					iterator->second.save(valueObj);
				}
				else if constexpr(std::is_enum<Value>::value)
				{
					valueObj = new JsonNode{ static_cast<int32_t>(iterator->second) };
				}
				else
					valueObj = new JsonNode{ iterator->second };

				mapObj->addNode("value", valueObj);
				map->addChild(mapObj);
			}
		}
		else if constexpr (std::is_pointer<typename CurrentField::Type >::value)
		{
			if ((CurrentField::get(obj)) != nullptr)
			{
				JsonNode* arrayObj { new JsonNode { JsonNode::EType::OBJECT } };
				root->addNode(CurrentField::name(), arrayObj);
				CurrentField::get(obj)->save(arrayObj);
			}
			else
				root->addNode(CurrentField::name(), new JsonNode { nullptr });
		}
		else if constexpr (std::is_enum<typename CurrentField::Type >::value)
			root->addNode(CurrentField::name(), new JsonNode { static_cast<int32_t>(CurrentField::get(obj)) });
		else if constexpr (std::is_scalar<typename CurrentField::Type>::value)
			root->addNode(CurrentField::name(), new JsonNode { CurrentField::get(obj) });
		else if constexpr (meta::Class<typename CurrentField::Type>::value)
		{
			JsonNode* arrayObj { new JsonNode { JsonNode::EType::OBJECT } };
			root->addNode(CurrentField::name(), arrayObj);
			CurrentField::get(obj).save(arrayObj);
		}
		else
			root->addNode(CurrentField::name(), new JsonNode { CurrentField::get(obj) });
	}

	template<typename T>
	template<uint32_t i>
	void ISerializable::Load<T>::invoke(T* obj, JsonNode* root)
	{
		typedef meta::Field<T, i> CurrentField;

		JsonNode* value { (*root)[CurrentField::name()] };
		if (!value)
			return;

		typedef typename CurrentField::Type FieldType;

		if constexpr (CurrentField::isArray())
		{
			for (uint32_t k { 0u } ; k < value->size() ; ++k)
			{
				JsonNode* data { (*value)[k] };
				const uint32_t type { (*data)["type"]->getUint32() };

				typedef typename FieldType::value_type Value;
				if constexpr (std::is_pointer<Value>::value)
				{
					if (registeredClass().count(type) == 0u)
						LOG(LogWarning, "loading field \'" << CurrentField::name() << "\': type \'" << meta::Type<Value>::sName << "\' (" << type << ") is not registered")
					else if (!registeredClass().at(type))
						LOG(LogWarning, "loading field \'" << CurrentField::name() << "\': type \'" << meta::Type<Value>::sName << "\' (" << type << ") is not constructible")
					else
						CurrentField::get(obj).emplace_back(static_cast<Value>(registeredClass().at(type)(data)));
				}
				else
					CurrentField::get(obj).emplace_back(data);
			}
		}
		else if constexpr (CurrentField::isMap())
		{
			for (uint32_t k{ 0u }; k < value->size(); ++k)
			{
				JsonNode* data{ (*value)[k] };

				// KEY
				JsonNode* keyMap{ (*data)["key"] };

				typedef typename FieldType::key_type Key;
				Key key{};
				if constexpr (!meta::Class<Key>::value)
					key = static_cast<typename std::remove_const<Key>::type>(keyMap->getUint32());
				else
					key = Key{ keyMap };

				// VALUE
				JsonNode* valueMap{ (*data)["value"] };
				typedef typename FieldType::mapped_type Value;
				if constexpr (std::is_pointer<Value>::value)
				{
					const uint32_t type{ (*valueMap)["type"]->getUint32() };
					if (registeredClass().count(type) == 0u)
						LOG(LogWarning, "loading field \'" << CurrentField::name() << "\': type \'" << meta::Type<Value>::sName << "\' (" << type << ") is not registered")
					else if (!registeredClass().at(type))
						LOG(LogWarning, "loading field \'" << CurrentField::name() << "\': type \'" << meta::Type<Value>::sName << "\' (" << type << ") is not constructible")
					else
						CurrentField::get(obj).emplace(key, static_cast<Value>(registeredClass()[type](valueMap)));
				}
				else if constexpr(meta::Class<Value>::value)
					CurrentField::get(obj).emplace(key, valueMap);
				else if constexpr(std::is_enum<Value>::value)
					CurrentField::get(obj).emplace(key, static_cast<typename std::remove_const<Value>::type>(valueMap->getUint32()));
				else
					CurrentField::get(obj).emplace(key, valueMap->getString());
			}
		}
		else if constexpr (std::is_pointer<FieldType>::value)
		{
			if (value->type() == JsonNode::EType::NULLPTR)
				CurrentField::set(obj, nullptr);
			else
			{
				const uint32_t type { (*value)["type"]->getUint32() };
				if (registeredClass().count(type) == 0u)
					LOG(LogWarning, "loading field \'" << CurrentField::name() << "\': type \'" << meta::Type<FieldType>::sName << "\' (" << type << ") is not registered")
				else if (!registeredClass().at(type))
					LOG(LogWarning, "loading field \'" << CurrentField::name() << "\': type \'" << meta::Type<FieldType>::sName << "\' (" << type << ") is not constructible")
				else
					CurrentField::set(obj, static_cast<FieldType>(registeredClass()[type](value)));
			}
		}
		else if constexpr (std::is_enum<FieldType>::value)
			CurrentField::set(obj, static_cast<typename std::remove_const<FieldType>::type>(value->getUint32()));
		else if constexpr(std::is_integral<FieldType>::value)
			CurrentField::set(obj, (value->type() == JsonNode::EType::BOOL ? value->getBool() : static_cast<FieldType>(value->getUint32())));
		else if constexpr(std::is_floating_point<FieldType>::value)
			CurrentField::set(obj, static_cast<FieldType>(value->getFloat()));
		else if constexpr (meta::Class<FieldType>::value)
			CurrentField::set(obj, typename CurrentField::Type(value));
		else
			CurrentField::set(obj, value->getString());
	}

	template<typename T>
	void ISerializable::serialize(JsonNode* root)
	{
		if constexpr (maj::meta::Class<T>::value)
		{
			root->addNode("type", new JsonNode { maj::meta::Class<T>::hash() });
			if constexpr (meta::Class<T>::size() > 0u)
				meta::staticFor<0u, meta::Class<T>::size() - 1u, Save<T>>(static_cast<T*>(this), root);
			else
				(void) root;
		}
	}

	template<typename T>
	void ISerializable::deserialize(JsonNode* root)
	{
		if constexpr (maj::meta::Class<T>::value)
		{
			if constexpr (meta::Class<T>::size() > 0u)
				meta::staticFor<0u, meta::Class<T>::size() - 1u, Load<T>>(static_cast<T*>(this), root);
			else
				(void) root;
		}
	}
}

namespace maj::meta
{
	template<typename T>
	Register<T>::Register()
	{
		ISerializable::registeredClass().try_emplace(maj::meta::Class<T>::hash(), [] (JsonNode* root) -> ISerializable*
													 {
														 return new T { root };
													 });
	};
}

#endif