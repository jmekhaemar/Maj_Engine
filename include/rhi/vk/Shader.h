#ifndef _SHADER_H_
#define _SHADER_H_

#include <vector>
#include <vulkan/vulkan.h>

#include <NonCopyable.h>
#include <ISerializable.h>
#include "common/Pipeline.h"

namespace maj::rhi
{
	struct Shader final : public NonCopyable, public ISerializable
	{
		ENABLE_META(Shader)

	private:
		struct Module : public NonCopyable
		{
			Module(const std::vector<char>& kCode);
			~Module();

			VkShaderModule mModule;
		};

		static std::vector<char> readFile(const char* kPath);

		static const char* const skShaderEntryPoint;

	public:
		LOAD_CONSTRUCTOR_CUSTOM(Shader)

		Shader(EShaderStageFlag shaderStage, const char* kPath);
		Shader(const Shader& kOther);
		Shader(Shader&& other);
		~Shader() = default;
		
		bool isValid() const;
		EShaderStageFlag stage() const;

		VkPipelineShaderStageCreateInfo info() const;

		Shader& operator=(Shader&& other);

	private:
		std::string mPath;
		EShaderStageFlag mShaderStage;
		std::shared_ptr<Module> mModule;
	};
}

META_CLASS(maj::rhi::Shader, 2u)
META_FIELD(maj::rhi::Shader, std::string, mPath, 0u)
META_FIELD(maj::rhi::Shader, maj::rhi::EShaderStageFlag, mShaderStage, 1u)

#endif