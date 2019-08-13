#ifndef _JSON_NODE_H_
#define _JSON_NODE_H_

#include <variant>
#include <vector>
#include <memory>
#include <unordered_map>
#include <iostream>

namespace maj
{
	class JsonNode final
	{
	private:
		typedef std::string StringType;
		typedef std::string NumberType;
		typedef bool BoolType;
		typedef void* NullType;
		typedef std::vector<JsonNode*> ArrayType;
		typedef std::unordered_map<std::string, JsonNode*> ObjectType;
		typedef std::variant<StringType, BoolType, NullType, ArrayType, ObjectType> NodeValue;

	public:
		enum class EType
		{
			STRING,
			NUMBER,
			BOOL,
			NULLPTR,
			ARRAY,
			OBJECT
		};

	public:
		static std::unique_ptr<JsonNode> parse(const char* kPath);

	public:
		JsonNode(EType type);
		JsonNode(StringType str);
		JsonNode(int32_t i);
		JsonNode(uint32_t u);
		JsonNode(int64_t i);
		JsonNode(uint64_t u);
		JsonNode(float f);
		JsonNode(double d);
		JsonNode(BoolType b);
		JsonNode(NullType n);
		~JsonNode();

		EType type() const;
		void save(const char* kPath);
	
		// StringType
		void setString(StringType str);
		StringType getString() const;

		// NumberType
		void setInt32(int32_t i);
		void setUint32(uint32_t u);
		void setInt64(int64_t i);
		void setUint64(uint64_t u);
		void setFloat(float f);
		void setDouble(double d);

		int32_t getInt32() const;
		uint32_t getUint32() const;
		int64_t getInt64() const;
		uint64_t getUint64() const;
		float getFloat() const;
		double getDouble() const;

		// BoolType
		void setBool(BoolType b);
		BoolType getBool() const;

		// NullType

		// ArrayType
		void addChild(JsonNode* node);
		void removeChild(uint32_t index);

		bool empty() const;
		size_t size() const;

		JsonNode* operator[](uint32_t index);

		// ObjectType
		void addNode(std::string kName, JsonNode*);
		void removeNode(std::string kName);

		ObjectType::iterator begin();
		ObjectType::iterator end();

		JsonNode* operator[] (std::string key);

	private:
		void save(std::ostream& os, uint16_t depth);

		const EType mkType;
		NodeValue mValue;
	};
}

#endif