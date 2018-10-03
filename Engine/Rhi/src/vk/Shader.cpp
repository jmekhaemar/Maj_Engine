#include "Shader.h"

#include "common/Logging.h"
#include "Device.h"
#include "VkHelper.h"

namespace maj::rhi
{
	DEFINE_HASH_OVERLOAD(Shader)

	const char* const Shader::skShaderEntryPoint{ "main" };

	Shader::Module::Module(const std::vector<char>& kCode)
		: mModule { VK_NULL_HANDLE }
	{
		VkShaderModuleCreateInfo createInfo { VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO };
		createInfo.codeSize = static_cast<uint32_t>(kCode.size());
		createInfo.pCode = reinterpret_cast<const uint32_t*>(kCode.data());

		VkResult result { vkCreateShaderModule(Service::get<Device>()->device(), &createInfo, nullptr, &mModule) };
		if (result != VK_SUCCESS)
			THROW("failed to create shader module with error: " + std::to_string(result))
	}

	Shader::Module::~Module()
	{
		if (mModule)
			vkDestroyShaderModule(Service::get<Device>()->device(), mModule, nullptr);
	}

	std::vector<char> Shader::readFile(const char* kPath)
	{
		std::ifstream file { kPath, std::ios::ate | std::ios::binary };
		if (!file.is_open())
			THROW("failed to read shader: can't open \'" + std::string { kPath } + "\'")

		uint32_t fileSize { static_cast<uint32_t>(file.tellg()) };
		std::vector<char> buffer {};
		buffer.resize(fileSize, '\0');

		file.seekg(0u);
		file.read(buffer.data(), fileSize);

		file.close();
		return buffer;
	}

	Shader::Shader(JsonNode* root)
		: mShaderStage{ 0u }
	{
		load(root);
		initialize();
	}

	Shader::Shader(EShaderStageFlag shaderStage, const char* kPath)
		: mPath { kPath }, mShaderStage{ shaderStage }, mModule {}
	{
		initialize();
	}
	
	Shader::Shader(const Shader& kOther)
		: mPath { kOther.mPath }, mShaderStage { kOther.mShaderStage }, mModule {}
	{
		initialize();
	}

	Shader::Shader(Shader&& other)
		: mPath { std::move(other.mPath) }, mShaderStage {other.mShaderStage }, mModule { other.mModule }
	{}

	void Shader::initialize()
	{
		if (mPath.empty())
		{
			LOG(LogRHIWarning, "failed to create shader: no path")
			return;
		}

		std::vector<char> code { readFile(mPath.c_str()) };
		mModule = std::make_shared<Module>(code);
	}

	bool Shader::isValid() const
	{
		return mModule && mModule->mModule != VK_NULL_HANDLE;
	}

	EShaderStageFlag Shader::stage() const
	{
		return mShaderStage;
	}

	VkPipelineShaderStageCreateInfo Shader::info() const
	{
		if (!mModule || mModule->mModule == VK_NULL_HANDLE)
			LOG(LogRHIWarning, "failed to get shader stage info: invalid shader");

		VkPipelineShaderStageCreateInfo info { VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO };
		info.stage = toShaderStageFlag(mShaderStage);
		info.module = mModule ? mModule->mModule : VK_NULL_HANDLE;
		info.pName = skShaderEntryPoint;

		return info;
	}

	Shader& Shader::operator=(Shader&& other)
	{
		mPath = std::move(other.mPath);
		mShaderStage = other.mShaderStage;
		mModule = std::move(other.mModule);

		return *this;
	}
}