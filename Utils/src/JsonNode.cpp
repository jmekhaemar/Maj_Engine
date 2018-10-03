#include "JsonNode.h"

#include <stack>
#include <fstream>

#include "Logging.h"

namespace maj
{
	std::unique_ptr<JsonNode> JsonNode::parse(const char* kPath)
	{
		JsonNode* root { nullptr };
		std::ifstream stream { kPath, std::ios::in };
		if (!stream.is_open())
			THROW("failed to parse json node: can't open file \'" + std::string { kPath } + "\'")

		char c {};
		char key[32u] {};
		std::stack<JsonNode*> stack {};

		while (!stream.eof())
		{
			do
			{
				c = static_cast<char>(stream.get());
			} while (c == ' ' || c == '\t');

			switch (c)
			{
				case '[':
				{
					JsonNode* node { new JsonNode { JsonNode::EType::ARRAY } };
					if (root)
						stack.top()->addChild(node);
					else
						root = node;
					stack.push(node);
					break;
				}
				case ']':
				{
					stack.pop();
					break;
				}
				case '{':
				{
					JsonNode* node { new JsonNode { JsonNode::EType::OBJECT } };
					if (root)
						stack.top()->addChild(node);
					else
						root = node;
					stack.emplace(node);
					break;
				}
				case '}':
				{
					stack.pop();
					break;
				}
				case '"':
				{
					stream.getline(key, 32, '"');
					break;
				}
				case ':':
				{
					while (true)
					{
						c = static_cast<char>(stream.peek());
						if (c != ' ' && c != '\t')
							break;
						stream.get();
					}
					std::string value;
					std::getline(stream, value);
					switch (c)
					{
					case '[':
					{
						JsonNode* node{ new JsonNode{ JsonNode::EType::ARRAY } };
						stack.top()->addNode(key, node);
						stack.emplace(node);
						break;
					}
					case '{':
					{
						JsonNode* node{ new JsonNode{ JsonNode::EType::OBJECT } };
						stack.top()->addNode(key, node);
						stack.emplace(node);
						break;
					}
					case '"':
					{
						if(value.back() == ',')
							value.pop_back();
						JsonNode* node{ new JsonNode{ value.substr(1u, value.size() - 2u) } };
						stack.top()->addNode(key, node);
						break;
					}
					case 'n':
					{
						JsonNode* node{ new JsonNode{ nullptr } };
						stack.top()->addNode(key, node);
						break;
					}
					case 't':
					{
						JsonNode* node{ new JsonNode{ true } };
						stack.top()->addNode(key, node);
						break;
					}
					case 'f':
					{
						JsonNode* node{ new JsonNode{ false } };
						stack.top()->addNode(key, node);
						break;
					}
					default:
					{
						if (value.back() == ',')
							value.pop_back();

						JsonNode* node{ new JsonNode{ EType::NUMBER } };
						node->mValue = value;
						stack.top()->addNode(key, node);
						break;
					}
					}
					break;
				}
			}
		}

		return std::unique_ptr<JsonNode>{ root };
	}

	JsonNode::JsonNode(EType type)
		: mkType{ type }
	{
		switch (mkType)
		{
			case EType::STRING:
			mValue = StringType();
			break;

			case EType::NUMBER:
			mValue = NumberType();
			break;

			case EType::BOOL:
			mValue = BoolType();
			break;

			case EType::NULLPTR:
			mValue = NullType();
			break;

			case EType::ARRAY:
			mValue = ArrayType();
			break;

			case EType::OBJECT:
			mValue = ObjectType();
			break;
		}
	}

	JsonNode::JsonNode(StringType str)
		: mkType{ EType::STRING }, mValue { str }
	{}

	JsonNode::JsonNode(int32_t i)
		: mkType{ EType::NUMBER }, mValue { std::to_string(i) }
	{}

	JsonNode::JsonNode(uint32_t u)
		: mkType{ EType::NUMBER }, mValue { std::to_string(u) }
	{}

	JsonNode::JsonNode(int64_t i)
		: mkType{ EType::NUMBER }, mValue { std::to_string(i) }
	{}

	JsonNode::JsonNode(uint64_t u)
		: mkType{ EType::NUMBER }, mValue { std::to_string(u) }
	{}

	JsonNode::JsonNode(float f)
		: mkType{ EType::NUMBER }, mValue { std::to_string(f) }
	{}

	JsonNode::JsonNode(double d)
		: mkType{ EType::NUMBER }, mValue { std::to_string(d) }
	{}

	JsonNode::JsonNode(BoolType b)
		: mkType{ EType::BOOL }, mValue { b }
	{}

	JsonNode::JsonNode(NullType n)
		: mkType{ EType::NULLPTR }, mValue { n }
	{}

	JsonNode::~JsonNode()
	{
		switch (mkType)
		{
			case EType::ARRAY:
			{
				ArrayType& arrayNodes { std::get<ArrayType>(mValue) };
				for (JsonNode* node : arrayNodes)
					delete node;
			}
			break;

			case EType::OBJECT:
			{
				ObjectType& object { std::get<ObjectType>(mValue) };
				for (std::pair<const std::string, JsonNode*>& pair : object)
					delete pair.second;
			}
			break;

			default:
			break;
		}
	}

	JsonNode::EType JsonNode::type() const
	{
		return mkType;
	}

	void JsonNode::save(const char* kPath)
	{
		std::ofstream stream{ kPath, std::ios::out };
		if (stream.is_open())
			save(stream, 0u);
		else
			LOG(LogWarning, "failed to save json node: can't open file " << kPath)
	}

	// StringType
	void JsonNode::setString(StringType str)
	{
		if (mkType == EType::STRING)
			mValue = str;
		else
			LOG(LogWarning, "failed to set json string: node is not a type of STRING")
	}

	JsonNode::StringType JsonNode::getString() const
	{
		if (mkType == EType::STRING)
			return std::get<StringType>(mValue);
		else
			LOG(LogWarning, "failed to get json string: node is not a type of STRING")
		return StringType();
	}

	// NumberType
	void JsonNode::setInt32(int32_t i)
	{
		if (mkType == EType::NUMBER)
			mValue = std::to_string(i);
		else
			LOG(LogWarning, "failed to set json int32: node is not a type of NUMBER")
	}

	void JsonNode::setUint32(uint32_t u)
	{
		if (mkType == EType::NUMBER)
			mValue = std::to_string(u);
		else
			LOG(LogWarning, "failed to set json uint32: node is not a type of NUMBER")
	}

	void JsonNode::setInt64(int64_t i)
	{
		if (mkType == EType::NUMBER)
			mValue = std::to_string(i);
		else
			LOG(LogWarning, "failed to set json int64: node is not a type of NUMBER")
	}

	void JsonNode::setUint64(uint64_t u)
	{
		if (mkType == EType::NUMBER)
			mValue = std::to_string(u);
		else
			LOG(LogWarning, "failed to set json uint64: node is not a type of NUMBER")
	}

	void JsonNode::setFloat(float f)
	{
		if (mkType == EType::NUMBER)
			mValue = std::to_string(f);
		else
			LOG(LogWarning, "failed to set json float: node is not a type of NUMBER")
	}

	void JsonNode::setDouble(double d)
	{
		if (mkType == EType::NUMBER)
			mValue = std::to_string(d);
		else
			LOG(LogWarning, "failed to set json double: node is not a type of NUMBER")
	}

	int32_t JsonNode::getInt32() const
	{
		if (mkType == EType::NUMBER)
			return static_cast<int32_t>(std::stoi(std::get<NumberType>(mValue)));
		else
			LOG(LogWarning, "failed to get json int32: node is not a type of NUMBER")
		return 0;
	}

	uint32_t JsonNode::getUint32() const
	{
		if (mkType == EType::NUMBER)
			return static_cast<uint32_t>(std::stoul(std::get<NumberType>(mValue)));
		else
			LOG(LogWarning, "failed to get json uint32: node is not a type of NUMBER")
		return 0u;
	}

	int64_t JsonNode::getInt64() const
	{
		if (mkType == EType::NUMBER)
			return static_cast<int64_t>(std::stoll(std::get<NumberType>(mValue)));
		else
			LOG(LogWarning, "failed to get json int64: node is not a type of NUMBER")
		return 0;
	}

	uint64_t JsonNode::getUint64() const
	{
		if (mkType == EType::NUMBER)
			return static_cast<uint64_t>(std::stoull(std::get<NumberType>(mValue)));
		else
			LOG(LogWarning, "failed to get json uint64: node is not a type of NUMBER")
		return 0u;
	}

	float JsonNode::getFloat() const
	{
		if (mkType == EType::NUMBER)
			return std::stof(std::get<NumberType>(mValue));
		else
			LOG(LogWarning, "failed to get json float: node is not a type of NUMBER")
		return 0.f;
	}

	double JsonNode::getDouble() const
	{
		if (mkType == EType::NUMBER)
			return std::stod(std::get<NumberType>(mValue));
		else
			LOG(LogWarning, "failed to get json double: node is not a type of NUMBER")
		return 0.f;
	}

	// BoolType
	void JsonNode::setBool(BoolType b)
	{
		if (mkType == EType::BOOL)
			mValue = b;
		else
			LOG(LogWarning, "failed to set json bool: node is not a type of BOOL")
	}

	JsonNode::BoolType JsonNode::getBool() const
	{
		if (mkType == EType::BOOL)
			return std::get<BoolType>(mValue);
		else
			LOG(LogWarning, "failed to get json bool: node is not a type of BOOL")
		return BoolType();
	}

	// NullType

	// Array Type

	void JsonNode::addChild(JsonNode* node)
	{
		if (mkType == EType::ARRAY)
			std::get<ArrayType>(mValue).emplace_back(node);
		else
			LOG(LogWarning, "failed to add json node child: node is not a type of ARRAY")
	}

	void JsonNode::removeChild(uint32_t index)
	{
		if (mkType == EType::ARRAY)
		{
			ArrayType& array { std::get<ArrayType>(mValue) };
			ArrayType::iterator it{ array.begin() + index }; 
			delete *it;
			array.erase(it);
		}
		else
			LOG(LogWarning, "failed to remove json node child: node is not a type of ARRAY")
	}

	bool JsonNode::empty() const
	{
		if (mkType == EType::ARRAY)
			return std::get<ArrayType>(mValue).empty();
		
		return false;
	}

	size_t JsonNode::size() const
	{
		if (mkType == EType::ARRAY)
			return std::get<ArrayType>(mValue).size();
		
		LOG(LogWarning, "failed to get json node size: node is not a type of ARRAY")
		return 0u;
	}

	JsonNode* JsonNode::operator[](uint32_t index)
	{
		if (mkType == EType::ARRAY)
			return std::get<ArrayType>(mValue)[index];
		
		LOG(LogWarning, "failed to get json node: node is not a type of ARRAY")
		return nullptr;
	}

	// ObjectType

	void JsonNode::addNode(std::string kName, JsonNode* node)
	{
		if (mkType == EType::OBJECT)
			std::get<ObjectType>(mValue)[kName] = node;
		else
			LOG(LogWarning, "failed to add json node: node is not a type of OBJECT")
	}

	void JsonNode::removeNode(std::string kName)
	{
		if (mkType == EType::OBJECT)
		{
			ObjectType& object { std::get<ObjectType>(mValue) };
			if (object.count(kName) == 0u)
			{
				LOG(LogWarning, "failed to get json node: node \'" << kName << "\' of type of OBJECT not found")
				return;
			}
				
			delete object.at(kName);
			object.erase(kName);
		}
		
		LOG(LogWarning, "failed to remove json node: node is not a type of OBJECT")
	}

	JsonNode::ObjectType::iterator JsonNode::begin()
	{
		if (mkType == EType::OBJECT)
			return std::get<ObjectType>(mValue).begin();
		else
			LOG(LogWarning, "failed to get json beginning: node is not a type of OBJECT")

		return JsonNode::ObjectType::iterator{};
	}

	JsonNode::ObjectType::iterator JsonNode::end()
	{
		if (mkType == EType::OBJECT)
			return std::get<ObjectType>(mValue).end();
		else
			LOG(LogWarning, "failed to get json end: node is not a type of OBJECT")

		return JsonNode::ObjectType::iterator {};
	}

	JsonNode* JsonNode::operator[](std::string key)
	{
		if (mkType == EType::OBJECT)
		{
			std::string k { key };
			ObjectType& object { std::get<ObjectType>(mValue) };
			if (object.count(key) == 0u)
			{
				LOG(LogWarning, "failed to get json node: node \'" << key << "\' of type of OBJECT not found")
				return nullptr;
			}

			return object.at(key);
		}
		
		LOG(LogWarning, "failed to get json node: node is not a type of OBJECT")
		return nullptr;
	}

	void JsonNode::save(std::ostream& os, uint16_t depth)
	{
		auto outDepth = [&](uint16_t dep) -> void
		{
			for (uint16_t i{ 0u }; i < dep; ++i)
				os << log::Logger<>::skTab;
		};

		switch (mkType)
		{
		case EType::STRING:
		{
			os << '\"' << std::get<StringType>(mValue) << '\"';
			break;
		}
		case EType::NUMBER:
		{
			os << std::get<NumberType>(mValue);
			break;
		}
		case EType::BOOL:
		{
			os << (std::get<BoolType>(mValue) ? "true" : "false");
			break;
		}
		case EType::NULLPTR:
		{
			os << "null";
			break;
		}
		case EType::ARRAY:
		{
			os << '[' << log::Logger<>::skEndl;

			ArrayType& array { std::get<ArrayType>(mValue) };
			ArrayType::iterator it { array.begin() };

			while (it != array.end())
			{
				(*it)->save(os, depth + 1);
				++it;

				if (it != array.end())
					os << ',';

				os << log::Logger<>::skEndl;
			}

			outDepth(depth);
			os << ']';
			break;
		}
		case EType::OBJECT:
		{
			outDepth(depth);
			os << '{' << log::Logger<>::skEndl;

			ObjectType& object { std::get<ObjectType>(mValue) };
			ObjectType::iterator it { object.begin() };

			while (it != object.end())
			{
				outDepth(depth + 1);

				os << '\"' << it->first << "\": ";
				it->second->save(os, depth + 1);
				++it;

				if (it != object.end())
					os << ',';

				os << log::Logger<>::skEndl;
			}

			outDepth(depth);
			os << '}';
			break;
		}
		}
	}
}