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
        VkShaderModuleCreateInfo CreateInfo = VkInit::ShaderModuleCreateInfo(SPIRVByteCode);

        VkShaderModule ShaderModule{};
        if (vkCreateShaderModule(Device, &CreateInfo, nullptr, &ShaderModule) != VK_SUCCESS)
        {
            CORVUS_CORE_CRITICAL("Failed to create Vulkan Shader Module!");
        }
        return ShaderModule;
    }

    void CRenderer::DestroyShaderModule(VkShaderModule &ShaderModule) const
    {
        if (ShaderModule)
        {
            vkDestroyShaderModule(Device, ShaderModule, nullptr);
            ShaderModule = VK_NULL_HANDLE;
        }
    }

} // namespace Corvus
