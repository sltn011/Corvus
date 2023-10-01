#include "CorvusPCH.h"

#include "Corvus/Renderer/Renderer.h"

namespace Corvus
{
    std::vector<char> CRenderer::ReadSPIRVByteCode(CString const &FilePath) const
    {
        std::ifstream File(FilePath, std::ios::ate | std::ios::binary);

        size_t            FileSize = static_cast<size_t>(File.tellg());
        std::vector<char> ByteCode(FileSize);

        File.seekg(0);
        File.read(ByteCode.data(), FileSize);

        return ByteCode;
    }

    VkShaderModule CRenderer::CreateShaderModule(std::vector<char> const &SPIRVByteCode) const
    {
        VkShaderModuleCreateInfo CreateInfo{};

        CreateInfo.sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        CreateInfo.codeSize = SPIRVByteCode.size();
        CreateInfo.pCode    = reinterpret_cast<uint32_t const *>(SPIRVByteCode.data());

        VkShaderModule ShaderModule{};
        if (vkCreateShaderModule(m_Device, &CreateInfo, nullptr, &ShaderModule) != VK_SUCCESS)
        {
            CORVUS_CRITICAL("Failed to create Vulkan Shader Module!");
        }
        return ShaderModule;
    }

    void CRenderer::DestroyShaderModule(VkShaderModule &ShaderModule) const
    {
        if (ShaderModule)
        {
            vkDestroyShaderModule(m_Device, ShaderModule, nullptr);
            ShaderModule = VK_NULL_HANDLE;
        }
    }

} // namespace Corvus
