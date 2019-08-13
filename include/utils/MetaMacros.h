#ifndef _META_MACROS_H_
#define _META_MACROS_H_

#include <typeinfo>
#include <unordered_map>

#include "JsonNode.h"

namespace maj::meta
{
	template<uint32_t start, uint32_t end, typename Func, uint32_t i = start, typename... Args>
	constexpr void staticFor(Args&&... args);

	constexpr uint32_t staticHash(const char* str);

	template<typename T>
	struct is_template : std::false_type {};

	template< template <typename... > class X, typename... P>
	struct is_template< X<P...> > : std::true_type {};

	template<typename T>
	struct Type final
	{
		static const char* sName;
		static size_t sSize;
	};

	template<typename Class, uint32_t i>
	struct Field final
	{};	

	template<typename T>
	struct Class final : public std::false_type
	{};

	template<typename T>
	struct Enum final
	{
		static constexpr uint32_t size();
	};

	template<typename T, uint32_t i>
	struct EnumValue final
	{};
}

#include "MetaMacros.inl"

#define META_FRIEND(CLASS)										\
		template<typename Class, uint32_t>						\
		friend struct maj::meta::Field;

#define REGISTER(CLASS)											\
		static inline maj::meta::Register<CLASS> __register {};

#define SAVE_FILE_SIGNATURE() virtual void save(const char* name) override
#define SAVE_JSON_SIGNATURE() virtual void save(maj::JsonNode* root) override
#define LOAD_JSON_SIGNATURE() virtual void load(maj::JsonNode* root) override

#define SAVE_FILE_CUSTOM_SIGNATURE(CLASS) void CLASS::save(const char* name)
#define SAVE_JSON_CUSTOM_SIGNATURE(CLASS) void CLASS::save(maj::JsonNode* root)
#define LOAD_JSON_CUSTOM_SIGNATURE(CLASS) void CLASS::load(maj::JsonNode* root)

#define INITIALIZE_SIGNATURE() virtual void initialize() override
#define INITIALIZE_CUSTOM_SIGNATURE(CLASS) void CLASS::initialize()

#define LOAD_CONSTRUCTOR_SIGNATURE(CLASS)	\
		REGISTER(CLASS)						\
		CLASS(maj::JsonNode* root)

#define SAVE_FILE_BODY()															\
		maj::JsonNode* root { new maj::JsonNode { maj::JsonNode::EType::OBJECT } };	\
		save(root);																	\
		root->save(name);															\
		delete root;

#define SAVE_JSON_BODY(CLASS)									\
		maj::ISerializable::template serialize<CLASS>(root);

#define	LOAD_JSON_BODY(CLASS)									\
		maj::ISerializable::template deserialize<CLASS>(root);

#define SAVE_JSON_CUSTOM_BODY(CLASS, PARENT)					\
		PARENT::save(root);										\
		maj::ISerializable::template serialize<CLASS>(root);

#define	LOAD_JSON_CUSTOM_BODY(CLASS, PARENT)					\
		PARENT::load(root);										\
		PARENT::initialize();									\
		maj::ISerializable::template deserialize<CLASS>(root);

#define LOAD_CONSTRUCTOR_CUSTOM(CLASS)			\
		INITIALIZE_SIGNATURE();					\
		LOAD_CONSTRUCTOR_SIGNATURE(CLASS);

#define LOAD_CONSTRUCTOR(CLASS)					\
		INITIALIZE_SIGNATURE();					\
		LOAD_CONSTRUCTOR_SIGNATURE(CLASS)		\
		{										\
			load(root);							\
			initialize();						\
		}

#define DEFINE_HASH_OVERLOAD(CLASS) uint32_t CLASS::hash(){ return meta::Class<CLASS>::hash(); }
#define HASH_OVERLOAD(CLASS) virtual uint32_t hash() override;				

#define ENABLE_META_CUSTOM(CLASS)		\
		META_FRIEND(CLASS)				\
		public:							\
		HASH_OVERLOAD(CLASS)			\
		SAVE_FILE_SIGNATURE();			\
		SAVE_JSON_SIGNATURE();			\
		LOAD_JSON_SIGNATURE();			\
		private:

#define ENABLE_META(CLASS)				\
		META_FRIEND(CLASS)				\
		public:							\
		HASH_OVERLOAD(CLASS)			\
		SAVE_FILE_SIGNATURE()			\
		{								\
			SAVE_FILE_BODY()			\
		}								\
		SAVE_JSON_SIGNATURE()			\
		{								\
			SAVE_JSON_BODY(CLASS)		\
		}								\
		LOAD_JSON_SIGNATURE()			\
		{								\
			LOAD_JSON_BODY(CLASS)		\
		}								\
		private:

#define ENABLE_META_PARENT(CLASS, PARENT)			\
		META_FRIEND(CLASS)							\
		public:										\
		HASH_OVERLOAD(CLASS)						\
		SAVE_FILE_SIGNATURE()						\
		{											\
			SAVE_FILE_BODY()						\
		}											\
		SAVE_JSON_SIGNATURE()						\
		{											\
			SAVE_JSON_CUSTOM_BODY(CLASS, PARENT)	\
		}											\
		LOAD_JSON_SIGNATURE()						\
		{											\
			LOAD_JSON_CUSTOM_BODY(CLASS, PARENT)	\
		}											\
		private:

#define META_CLASS_(CLASS, PARENT, NUM, HAS_PARENT)								\
		template<>																\
		struct maj::meta::Class<CLASS> final : public std::true_type			\
		{																		\
			typedef PARENT Parent;												\
			static constexpr const char* name() { return #CLASS; }				\
			static constexpr uint32_t hash() { return staticHash(#CLASS); }		\
			static constexpr bool hasParent() { return HAS_PARENT; }			\
			static constexpr uint32_t size() { return NUM; }					\
		};																		

#define META_CLASS(CLASS, NUM)					META_CLASS_(CLASS, void, NUM, false)
#define META_CLASS_PARENT(CLASS, PARENT, NUM)	META_CLASS_(CLASS, PARENT, NUM, true)

#define META_FIELD_(CLASS, TYPE, FIELD, NUM, IS_ARRAY, IS_MAP)																\
		template<>																											\
		struct maj::meta::Field<CLASS, NUM> final 																			\
		{																													\
			typedef TYPE Type;																								\
			static constexpr bool isArray(){ return IS_ARRAY; }																\
			static constexpr bool isMap(){ return IS_MAP; }																	\
			static constexpr const char* name(){ return #FIELD; }															\
			static constexpr Type CLASS::* pointer(){ return &CLASS::FIELD; }												\
			static constexpr Type& get(CLASS* obj)																			\
			{																												\
				return obj->*pointer();																						\
			}																												\
			static void set(CLASS* obj, std::remove_const<Type>::type&& field)												\
			{																												\
				const_cast<std::remove_const<Type>::type&>(get(obj)) = std::forward<std::remove_const<Type>::type>(field);	\
			}																												\
		};

#define META_FIELD(CLASS, TYPE, FIELD, NUM)			META_FIELD_(CLASS, TYPE, FIELD, NUM, false, false)
#define META_FIELD_ARRAY(CLASS, TYPE, FIELD, NUM)	META_FIELD_(CLASS, TYPE, FIELD, NUM, true, false)
#define META_FIELD_MAP(CLASS, TYPE, FIELD, NUM)		META_FIELD_(CLASS, TYPE, FIELD, NUM, false, true)

#define META_ENUM_(ENUM, NUM, FLAG)														\
		template<>																		\
		struct maj::meta::Enum<ENUM> final												\
		{																				\
		private:																		\
			struct GetName																\
			{																			\
				template<uint32_t i>													\
				static void invoke(uint32_t value, const char*& str)					\
				{																		\
					if(i == value)														\
						str = EnumValue<ENUM, i>::name();								\
				}																		\
			};																			\
			struct GetValue																\
			{																			\
				template<uint32_t i>													\
				static void invoke(uint32_t value, ENUM& val)							\
				{																		\
					if(i == value)														\
						val = EnumValue<ENUM, i>::value();								\
				}																		\
			};																			\
		public:																			\
			static constexpr bool isFlag(){ return FLAG; }								\
			static constexpr const char* name(){ return #ENUM; }						\
			static constexpr uint32_t size() { return NUM; }							\
			static constexpr const char* getName(uint32_t i) {							\
				const char* str{};														\
				maj::meta::staticFor<0u, size() - 1u, GetName>(i, str);					\
				return str;																\
			}																			\
			static constexpr ENUM getValue(uint32_t i) {								\
				ENUM value{};															\
				maj::meta::staticFor<0u, size() - 1u, GetValue>(i, value);				\
				return value;															\
			}																			\
		};												

#define META_ENUM(ENUM, NUM) META_ENUM_(ENUM, NUM, false)
#define META_FLAG(ENUM, NUM) META_ENUM_(ENUM, NUM, true)

#define META_ENUM_VALUE_(ENUM, VALUE, INDEX)									\
		template<>																\
		struct maj::meta::EnumValue<ENUM, INDEX> final							\
		{																		\
			static constexpr const char* name(){ return #VALUE; }				\
			static constexpr ENUM value(){ return VALUE; }						\
		};											

#define META_ENUM_VALUE(ENUM, VALUE) META_ENUM_VALUE_(ENUM, VALUE, static_cast<uint32_t>(VALUE))
#define META_FLAG_VALUE(ENUM, VALUE, INDEX) META_ENUM_VALUE_(ENUM, VALUE, INDEX)

#endif