#ifndef CORVUS_SOURCE_CORVUS_RENDERER_INITIALIZERS_H
#define CORVUS_SOURCE_CORVUS_RENDERER_INITIALIZERS_H

#include "Corvus/Core/Base.h"
#include "Corvus/Renderer/Core/VulkanSwapchainSupportDetails.h"

#include <vulkan/vulkan.h>

namespace Corvus
{

    namespace VkInit
    {

        inline VkApplicationInfo ApplicationInfo(
            CString const &ApplicationName, UInt32 ApplicationVersion, CString const &EngineName, UInt32 EngineVersion
        )
        {
            VkApplicationInfo ApplicationInfo{};
            ApplicationInfo.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
            ApplicationInfo.apiVersion         = VK_API_VERSION_1_3;
            ApplicationInfo.pApplicationName   = ApplicationName.c_str();
            ApplicationInfo.applicationVersion = ApplicationVersion;
            ApplicationInfo.pEngineName        = EngineName.c_str();
            ApplicationInfo.engineVersion      = EngineVersion;
            return ApplicationInfo;
        }

        inline VkInstanceCreateInfo InstanceCreateInfo(
            VkApplicationInfo               &ApplicationInfo,
            std::vector<char const *> const &Extensions,
            std::vector<char const *> const &ValidationLayers
        )
        {
            VkInstanceCreateInfo InstanceCreateInfo{};
            InstanceCreateInfo.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
            InstanceCreateInfo.pApplicationInfo        = &ApplicationInfo;
            InstanceCreateInfo.enabledExtensionCount   = static_cast<UInt32>(Extensions.size());
            InstanceCreateInfo.ppEnabledExtensionNames = Extensions.data();
            InstanceCreateInfo.enabledLayerCount       = static_cast<UInt32>(ValidationLayers.size());
            InstanceCreateInfo.ppEnabledLayerNames     = ValidationLayers.data();
            return InstanceCreateInfo;
        }

        inline VkDebugUtilsMessengerCreateInfoEXT DebugUtilsMessengerCreateInfo(
            PFN_vkDebugUtilsMessengerCallbackEXT DebugMessageCallback
        )
        {
            VkDebugUtilsMessengerCreateInfoEXT MessengerInfo{};
            MessengerInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
            MessengerInfo.messageSeverity =
                VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
                VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
            MessengerInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                                        VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                                        VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
            MessengerInfo.pfnUserCallback = DebugMessageCallback;
            MessengerInfo.pUserData       = nullptr;
            return MessengerInfo;
        }

        inline VkDeviceQueueCreateInfo DeviceQueueCreateInfo(
            UInt32 QueueFamilyIndex, float const *pQueuePriorities, SizeT NumQueuePriorities
        )
        {
            VkDeviceQueueCreateInfo DeviceQueueCreateInfo{};
            DeviceQueueCreateInfo.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            DeviceQueueCreateInfo.queueFamilyIndex = QueueFamilyIndex;
            DeviceQueueCreateInfo.queueCount       = static_cast<UInt32>(NumQueuePriorities);
            DeviceQueueCreateInfo.pQueuePriorities = pQueuePriorities;
            return DeviceQueueCreateInfo;
        }

        inline VkDeviceCreateInfo DeviceCreateInfo(
            std::vector<VkDeviceQueueCreateInfo> const &QueuesCreateInfo,
            VkPhysicalDeviceFeatures                   *pDeviceRequestedFeatures,
            std::vector<char const *> const            &Extensions,
            std::vector<char const *> const            &ValidationLayers
        )
        {
            VkDeviceCreateInfo DeviceCreateInfo{};
            DeviceCreateInfo.sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
            DeviceCreateInfo.pQueueCreateInfos       = QueuesCreateInfo.data();
            DeviceCreateInfo.queueCreateInfoCount    = static_cast<UInt32>(QueuesCreateInfo.size());
            DeviceCreateInfo.pEnabledFeatures        = pDeviceRequestedFeatures;
            DeviceCreateInfo.enabledExtensionCount   = static_cast<UInt32>(Extensions.size());
            DeviceCreateInfo.ppEnabledExtensionNames = Extensions.data();
            DeviceCreateInfo.enabledLayerCount       = static_cast<UInt32>(ValidationLayers.size());
            DeviceCreateInfo.ppEnabledLayerNames     = ValidationLayers.data();
            return DeviceCreateInfo;
        }

        inline VkSwapchainCreateInfoKHR SwapchainCreateInfo(
            VkSurfaceKHR       Surface,
            UInt32             ImagesCount,
            VkExtent2D         Extent,
            VkSurfaceFormatKHR SurfaceFormat,
            VkPresentModeKHR   PresentMode,
            VkSharingMode      SharingMode,
            UInt32 const      *pQueueFamiliesIndices, // Can be empty if sharing mode == VK_SHARING_MODE_EXCLUSIVE
            SizeT              NumQueueFamilyIndices,
            CVulkanSwapchainSupportDetails SupportDetails
        )
        {
            VkSwapchainCreateInfoKHR CreateInfo{};
            CreateInfo.sType            = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
            CreateInfo.surface          = Surface;
            CreateInfo.minImageCount    = ImagesCount;
            CreateInfo.imageExtent      = Extent;
            CreateInfo.imageFormat      = SurfaceFormat.format;
            CreateInfo.imageColorSpace  = SurfaceFormat.colorSpace;
            CreateInfo.presentMode      = PresentMode;
            CreateInfo.imageArrayLayers = 1; // Unless it's a stereoscopic 3D app
            CreateInfo.imageUsage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
            CreateInfo.preTransform     = SupportDetails.SurfaceCapabilities.currentTransform; // Don't want transforms
            CreateInfo.compositeAlpha   = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;                   // treat alpha as 1.0f
            CreateInfo.clipped          = VK_TRUE; // Ignore obstructed pixels
            CreateInfo.oldSwapchain     = VK_NULL_HANDLE;

            if (SharingMode == VK_SHARING_MODE_CONCURRENT)
            {
                CreateInfo.queueFamilyIndexCount = static_cast<UInt32>(NumQueueFamilyIndices);
                CreateInfo.pQueueFamilyIndices   = pQueueFamiliesIndices;
            }
            else
            {
                CreateInfo.queueFamilyIndexCount = 0;       // Optional
                CreateInfo.pQueueFamilyIndices   = nullptr; // Optional
            }

            return CreateInfo;
        }

        inline VkCommandPoolCreateInfo CommandPoolCreateInfo(UInt32 QueueFamilyIndex, VkCommandPoolCreateFlags Flags)
        {
            VkCommandPoolCreateInfo CommandPoolInfo{};
            CommandPoolInfo.sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
            CommandPoolInfo.flags            = Flags;
            CommandPoolInfo.queueFamilyIndex = QueueFamilyIndex;
            return CommandPoolInfo;
        }

        inline VkCommandBufferAllocateInfo CommandBufferAllocateInfo(VkCommandPool CommandPool, UInt32 Count)
        {
            VkCommandBufferAllocateInfo AllocateInfo{};
            AllocateInfo.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            AllocateInfo.commandPool        = CommandPool;
            AllocateInfo.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
            AllocateInfo.commandBufferCount = Count;
            return AllocateInfo;
        }

        inline VkCommandBufferBeginInfo CommandBufferBeginInfo(VkCommandBufferUsageFlags Flags)
        {
            VkCommandBufferBeginInfo BeginInfo{};
            BeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            BeginInfo.flags = Flags;
            return BeginInfo;
        }

        inline VkSubmitInfo SubmitInfo(
            VkCommandBuffer const      &CommandBuffer,
            VkSemaphore const          *pWaitSemaphores,
            SizeT                       NumWaitSemaphores,
            VkPipelineStageFlags const *pWaitStages,
            VkSemaphore const          *pSignalSemaphores,
            SizeT                       NumSignalSemaphores
        )
        {
            VkSubmitInfo SubmitInfo{};
            SubmitInfo.sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            SubmitInfo.commandBufferCount   = 1;
            SubmitInfo.pCommandBuffers      = &CommandBuffer;
            SubmitInfo.waitSemaphoreCount   = static_cast<UInt32>(NumWaitSemaphores);
            SubmitInfo.pWaitSemaphores      = pWaitSemaphores;
            SubmitInfo.pWaitDstStageMask    = pWaitStages;
            SubmitInfo.signalSemaphoreCount = static_cast<UInt32>(NumSignalSemaphores);
            SubmitInfo.pSignalSemaphores    = pSignalSemaphores;
            return SubmitInfo;
        }

        inline VkDescriptorSetLayoutBinding DescriptorSetLayoutBinding(
            UInt32 Binding, VkDescriptorType DescriptorType, UInt32 Count, VkShaderStageFlags ShaderStageFlags
        )
        {
            VkDescriptorSetLayoutBinding LayoutBinding{};
            LayoutBinding.binding            = Binding;
            LayoutBinding.descriptorType     = DescriptorType;
            LayoutBinding.descriptorCount    = Count; // > 1 to make uniform array
            LayoutBinding.stageFlags         = ShaderStageFlags;
            LayoutBinding.pImmutableSamplers = nullptr; // not needed here
            return LayoutBinding;
        }

        inline VkDescriptorSetLayoutCreateInfo DescriptorSetLayoutCreateInfo(
            VkDescriptorSetLayoutBinding const *pBindings, SizeT NumBindings
        )
        {
            VkDescriptorSetLayoutCreateInfo DescriptorSetLayoutInfo{};
            DescriptorSetLayoutInfo.sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
            DescriptorSetLayoutInfo.bindingCount = static_cast<UInt32>(NumBindings);
            DescriptorSetLayoutInfo.pBindings    = pBindings;
            return DescriptorSetLayoutInfo;
        }

        inline VkDescriptorPoolCreateInfo DescriptorPoolCreateInfo(
            VkDescriptorPoolSize const *pPoolSizes,
            SizeT                       NumPoolSizes,
            UInt32                      MaxSets,
            VkDescriptorPoolCreateFlags Flags
        )
        {
            VkDescriptorPoolCreateInfo DescriptorPoolInfo{};
            DescriptorPoolInfo.sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
            DescriptorPoolInfo.poolSizeCount = static_cast<UInt32>(NumPoolSizes);
            DescriptorPoolInfo.pPoolSizes    = pPoolSizes;
            DescriptorPoolInfo.maxSets       = MaxSets;
            DescriptorPoolInfo.flags         = Flags;
            return DescriptorPoolInfo;
        }

        inline VkDescriptorSetAllocateInfo DescriptorSetAllocateInfo(
            VkDescriptorPool DescriptorPool, VkDescriptorSetLayout const *pDescriptorSetsLayouts, SizeT NumSetsLayouts
        )
        {
            VkDescriptorSetAllocateInfo AllocateInfo{};
            AllocateInfo.sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
            AllocateInfo.descriptorPool     = DescriptorPool;
            AllocateInfo.descriptorSetCount = static_cast<UInt32>(NumSetsLayouts);
            AllocateInfo.pSetLayouts        = pDescriptorSetsLayouts;
            return AllocateInfo;
        }

        inline VkDescriptorBufferInfo DescriptorBufferInfo(VkBuffer Buffer, VkDeviceSize Offset, VkDeviceSize Range)
        {
            VkDescriptorBufferInfo DescriptorBufferInfo{};
            DescriptorBufferInfo.buffer = Buffer;
            DescriptorBufferInfo.offset = Offset;
            DescriptorBufferInfo.range  = Range;
            return DescriptorBufferInfo;
        }

        inline VkWriteDescriptorSet WriteDescriptorSet(
            VkDescriptorSet               DescriptorSet,
            UInt32                        Binding,
            UInt32                        ArrayElement,
            VkDescriptorType              DescriptorType,
            VkDescriptorBufferInfo const *pDescriptorBuffersInfo,
            VkDescriptorImageInfo const  *pDescriptorImagesInfo,
            VkBufferView const           *pTexelBufferViews,
            SizeT                         NumDescriptorInfos
        )
        {
            VkWriteDescriptorSet DescriptorSetWrite{};
            DescriptorSetWrite.sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            DescriptorSetWrite.dstSet           = DescriptorSet;
            DescriptorSetWrite.dstBinding       = 0;
            DescriptorSetWrite.dstArrayElement  = 0;
            DescriptorSetWrite.descriptorType   = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            DescriptorSetWrite.descriptorCount  = static_cast<UInt32>(NumDescriptorInfos);
            DescriptorSetWrite.pBufferInfo      = pDescriptorBuffersInfo;
            DescriptorSetWrite.pImageInfo       = pDescriptorImagesInfo;
            DescriptorSetWrite.pTexelBufferView = pTexelBufferViews;
            return DescriptorSetWrite;
        }

        inline VkAttachmentDescription AttachmentDescription(
            VkFormat            Format,
            VkImageLayout       InitialLayout,
            VkImageLayout       FinalLayout,
            VkAttachmentLoadOp  LoadOp,
            VkAttachmentStoreOp StoreOp,
            VkAttachmentLoadOp  StencilLoadOp,
            VkAttachmentStoreOp StencilStoreOp
        )
        {
            VkAttachmentDescription Attachment{};
            Attachment.format         = Format;
            Attachment.samples        = VK_SAMPLE_COUNT_1_BIT;
            Attachment.loadOp         = LoadOp;
            Attachment.storeOp        = StoreOp;
            Attachment.stencilLoadOp  = StencilLoadOp;
            Attachment.stencilStoreOp = StencilStoreOp;
            Attachment.initialLayout  = InitialLayout;
            Attachment.finalLayout    = FinalLayout;
            return Attachment;
        }

        inline VkAttachmentReference AttachmentReference(UInt32 Index, VkImageLayout Layout)
        {
            VkAttachmentReference AttachmentRef{};
            AttachmentRef.attachment = Index; // index of attachment in pAttachments array in RenderPassInfo
            AttachmentRef.layout     = Layout;
            return AttachmentRef;
        }

        inline VkSubpassDescription SubpassDescription(
            VkPipelineBindPoint          PipelineBindPoint,
            VkAttachmentReference const *pColorAttachmentsRefs,
            SizeT                        NumColorAttachmentsRefs,
            VkAttachmentReference       *pDepthStencilAttachmentRef
        )
        {
            VkSubpassDescription Subpass{};
            Subpass.pipelineBindPoint       = PipelineBindPoint;
            Subpass.colorAttachmentCount    = static_cast<UInt32>(NumColorAttachmentsRefs);
            Subpass.pColorAttachments       = pColorAttachmentsRefs;
            Subpass.pDepthStencilAttachment = pDepthStencilAttachmentRef;
            return Subpass;
        }

        inline VkSubpassDependency SubpassDependency(
            UInt32               SrcSubpass,
            UInt32               DstSubpass,
            VkPipelineStageFlags SrcStageMask,
            VkAccessFlags        SrcAccessMask,
            VkPipelineStageFlags DstStageMask,
            VkAccessFlags        DstAccessMask
        )
        {
            VkSubpassDependency Dependency{};
            Dependency.srcSubpass    = SrcSubpass;
            Dependency.dstSubpass    = DstSubpass;
            Dependency.srcStageMask  = SrcStageMask;
            Dependency.srcAccessMask = SrcAccessMask;
            Dependency.dstStageMask  = DstStageMask;
            Dependency.dstAccessMask = DstAccessMask;
            return Dependency;
        }

        inline VkRenderPassCreateInfo RenderPassCreateInfo(
            VkAttachmentDescription const *pAttachmentsDescriptions,
            SizeT                          NumAttachmentsDescriptions,
            VkSubpassDescription const    *pSubpassesDescriptions,
            SizeT                          NumSubpassesDescriptions,
            VkSubpassDependency const     *pDependenciesDescriptions,
            SizeT                          NumDependenciesDescriptions
        )
        {
            VkRenderPassCreateInfo RenderPassInfo{};
            RenderPassInfo.sType           = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
            RenderPassInfo.attachmentCount = static_cast<UInt32>(NumAttachmentsDescriptions);
            RenderPassInfo.pAttachments    = pAttachmentsDescriptions;
            RenderPassInfo.subpassCount    = static_cast<UInt32>(NumSubpassesDescriptions);
            RenderPassInfo.pSubpasses      = pSubpassesDescriptions;
            RenderPassInfo.dependencyCount = static_cast<UInt32>(NumDependenciesDescriptions);
            RenderPassInfo.pDependencies   = pDependenciesDescriptions;
            return RenderPassInfo;
        }

        inline VkRenderPassBeginInfo RenderPassBeginInfo(
            VkRenderPass        RenderPass,
            VkFramebuffer       Framebuffer,
            VkExtent2D          Extent,
            VkClearValue const *pClearColors,
            SizeT               NumClearColors
        )
        {
            VkRenderPassBeginInfo BeginInfo{};
            BeginInfo.sType             = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            BeginInfo.renderPass        = RenderPass;
            BeginInfo.framebuffer       = Framebuffer;
            BeginInfo.renderArea.offset = {0, 0};
            BeginInfo.renderArea.extent = Extent;
            BeginInfo.clearValueCount   = static_cast<UInt32>(NumClearColors);
            BeginInfo.pClearValues      = pClearColors;
            return BeginInfo;
        }

        inline VkViewport Viewport(VkExtent2D Extent)
        {
            VkViewport ViewportInfo{};
            ViewportInfo.x        = 0.0f;
            ViewportInfo.y        = 0.0f;
            ViewportInfo.width    = static_cast<float>(Extent.width);
            ViewportInfo.height   = static_cast<float>(Extent.height);
            ViewportInfo.minDepth = 0.0f;
            ViewportInfo.maxDepth = 1.0f;
            return ViewportInfo;
        }

        inline VkRect2D Scissor(VkExtent2D Extent)
        {
            VkRect2D ScissorInfo{};
            ScissorInfo.offset = {0, 0};
            ScissorInfo.extent = Extent;
            return ScissorInfo;
        }

        inline VkPushConstantRange PushConstantRange(UInt32 Offset, UInt32 Size, VkShaderStageFlags ShaderStageFlags)
        {
            VkPushConstantRange PushConstantRange{};
            PushConstantRange.offset     = Offset;
            PushConstantRange.size       = Size;
            PushConstantRange.stageFlags = ShaderStageFlags;
            return PushConstantRange;
        }

        inline VkPipelineLayoutCreateInfo PipelineLayoutCreateInfo(
            VkDescriptorSetLayout const *pDescriptorSetsLayouts,
            SizeT                        NumSetsLayouts,
            VkPushConstantRange const   *pPushConstantsRanges,
            SizeT                        NumPushConstantsRanges
        )
        {
            VkPipelineLayoutCreateInfo PipelineLayoutInfo{};
            PipelineLayoutInfo.sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
            PipelineLayoutInfo.setLayoutCount         = static_cast<UInt32>(NumSetsLayouts);
            PipelineLayoutInfo.pSetLayouts            = pDescriptorSetsLayouts;
            PipelineLayoutInfo.pushConstantRangeCount = static_cast<UInt32>(NumPushConstantsRanges);
            PipelineLayoutInfo.pPushConstantRanges    = pPushConstantsRanges;
            return PipelineLayoutInfo;
        }

        inline VkShaderModuleCreateInfo ShaderModuleCreateInfo(std::vector<char> const &SPIRVByteCode)
        {
            VkShaderModuleCreateInfo CreateInfo{};
            CreateInfo.sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
            CreateInfo.codeSize = SPIRVByteCode.size();
            CreateInfo.pCode    = reinterpret_cast<UInt32 const *>(SPIRVByteCode.data());
            return CreateInfo;
        }

        inline VkPipelineShaderStageCreateInfo PipelineShaderStageCreateInfo(
            VkShaderModule ShaderModule, VkShaderStageFlagBits ShaderStage
        )
        {
            VkPipelineShaderStageCreateInfo ShaderStageInfo{};
            ShaderStageInfo.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            ShaderStageInfo.stage  = ShaderStage;
            ShaderStageInfo.module = ShaderModule;
            ShaderStageInfo.pName  = "main";
            return ShaderStageInfo;
        }

        inline VkPipelineVertexInputStateCreateInfo PipelineVertexInputStateInfo(
            VkVertexInputBindingDescription const   *pVertexInputBindingDescription,
            SizeT                                    NumVertexInputBindingsDescriptions,
            VkVertexInputAttributeDescription const *pVertexInputAttributeDescriptions,
            SizeT                                    NumVertexInputAttributeDescriptions
        )
        {
            VkPipelineVertexInputStateCreateInfo VertexInputStateInfo{};
            VertexInputStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
            VertexInputStateInfo.vertexBindingDescriptionCount =
                static_cast<UInt32>(NumVertexInputBindingsDescriptions);
            VertexInputStateInfo.pVertexBindingDescriptions = pVertexInputBindingDescription;
            VertexInputStateInfo.vertexAttributeDescriptionCount =
                static_cast<UInt32>(NumVertexInputAttributeDescriptions);
            VertexInputStateInfo.pVertexAttributeDescriptions = pVertexInputAttributeDescriptions;
            return VertexInputStateInfo;
        }

        inline VkPipelineInputAssemblyStateCreateInfo PipelineInputAssemblyStateCreateInfo()
        {
            VkPipelineInputAssemblyStateCreateInfo InputAssemblyStageInfo{};
            InputAssemblyStageInfo.sType                  = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
            InputAssemblyStageInfo.topology               = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
            InputAssemblyStageInfo.primitiveRestartEnable = VK_FALSE;
            return InputAssemblyStageInfo;
        }

        inline VkPipelineDynamicStateCreateInfo PipelineDynamicStateCreateInfo(
            VkDynamicState const *pDynamicStates, SizeT NumDynamicStates
        )
        {
            VkPipelineDynamicStateCreateInfo DynamicStateInfo{};
            DynamicStateInfo.sType             = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
            DynamicStateInfo.dynamicStateCount = static_cast<UInt32>(NumDynamicStates);
            DynamicStateInfo.pDynamicStates    = pDynamicStates;
            return DynamicStateInfo;
        }

        inline VkPipelineViewportStateCreateInfo PipelineViewportStateCreateInfo()
        {
            VkPipelineViewportStateCreateInfo ViewportState{};
            ViewportState.sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
            ViewportState.viewportCount = 1;
            ViewportState.pViewports    = nullptr; // ignored if dynamic
            ViewportState.scissorCount  = 1;
            ViewportState.pScissors     = nullptr; // ignored if dynamic
            return ViewportState;
        }

        inline VkPipelineRasterizationStateCreateInfo PipelineRasterizationStateCreateInfo(bool bCullBackface)
        {
            VkCullModeFlagBits CullMode = bCullBackface ? VK_CULL_MODE_BACK_BIT : VK_CULL_MODE_NONE;

            VkPipelineRasterizationStateCreateInfo RasterizerStageInfo{};
            RasterizerStageInfo.sType                   = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
            RasterizerStageInfo.depthClampEnable        = VK_FALSE;
            RasterizerStageInfo.rasterizerDiscardEnable = VK_FALSE;
            RasterizerStageInfo.polygonMode             = VK_POLYGON_MODE_FILL;
            RasterizerStageInfo.lineWidth               = 1.0f;
            RasterizerStageInfo.cullMode                = CullMode;
            RasterizerStageInfo.frontFace               = VK_FRONT_FACE_COUNTER_CLOCKWISE;
            RasterizerStageInfo.depthBiasEnable         = VK_FALSE;
            RasterizerStageInfo.depthBiasConstantFactor = 0.0f; // optional
            RasterizerStageInfo.depthBiasClamp          = 0.0f; // optional
            RasterizerStageInfo.depthBiasSlopeFactor    = 0.0f; // optional
                                                                // Frontface swapped bcz of flipped Y in proj
            return RasterizerStageInfo;
        }

        inline VkPipelineMultisampleStateCreateInfo PipelineMultisampleStateCreateInfo()
        {
            VkPipelineMultisampleStateCreateInfo MultisamplerStateInfo{};
            MultisamplerStateInfo.sType                 = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
            MultisamplerStateInfo.sampleShadingEnable   = VK_FALSE;
            MultisamplerStateInfo.rasterizationSamples  = VK_SAMPLE_COUNT_1_BIT;
            MultisamplerStateInfo.minSampleShading      = 1.0f;     // optional
            MultisamplerStateInfo.pSampleMask           = nullptr;  // optional
            MultisamplerStateInfo.alphaToCoverageEnable = VK_FALSE; // optional
            MultisamplerStateInfo.alphaToOneEnable      = VK_FALSE; // optional
            return MultisamplerStateInfo;
        }

        inline VkPipelineDepthStencilStateCreateInfo PipelineDepthStencilStateCreateInfo(
            bool bDepthTestEnabled, bool bDepthTestWrite
        )
        {
            VkBool32 bDoDepthTest  = bDepthTestEnabled ? VK_TRUE : VK_FALSE;
            VkBool32 bDoDepthWrite = bDepthTestWrite ? VK_TRUE : VK_FALSE;

            VkPipelineDepthStencilStateCreateInfo DepthStencilTestStateInfo{};
            DepthStencilTestStateInfo.sType            = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
            DepthStencilTestStateInfo.depthTestEnable  = bDoDepthTest;
            DepthStencilTestStateInfo.depthWriteEnable = bDoDepthWrite;
            DepthStencilTestStateInfo.depthCompareOp   = VK_COMPARE_OP_LESS;
            DepthStencilTestStateInfo.depthBoundsTestEnable = VK_FALSE;
            DepthStencilTestStateInfo.minDepthBounds        = 0.0f;
            DepthStencilTestStateInfo.maxDepthBounds        = 1.0f;
            DepthStencilTestStateInfo.stencilTestEnable     = VK_FALSE;
            DepthStencilTestStateInfo.front                 = {};
            DepthStencilTestStateInfo.back                  = {};
            return DepthStencilTestStateInfo;
        }

        inline VkPipelineColorBlendAttachmentState PipelineColorBlendAttachmentState()
        {
            VkPipelineColorBlendAttachmentState ColorBlendAttachment{};
            ColorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
                                                  VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
            ColorBlendAttachment.blendEnable         = VK_FALSE;
            ColorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;  // optional
            ColorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // optional
            ColorBlendAttachment.colorBlendOp        = VK_BLEND_OP_ADD;      // optional
            ColorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;  // optional
            ColorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // optional
            ColorBlendAttachment.alphaBlendOp        = VK_BLEND_OP_ADD;      // optional
            return ColorBlendAttachment;
        }

        inline VkPipelineColorBlendStateCreateInfo PipelineColorBlendStateCreateInfo(
            VkPipelineColorBlendAttachmentState const *pColorBlendAttachments, SizeT NumColorBlendAttachments
        )
        {
            VkPipelineColorBlendStateCreateInfo ColorBlendState{};
            ColorBlendState.sType             = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
            ColorBlendState.logicOpEnable     = VK_FALSE;
            ColorBlendState.logicOp           = VK_LOGIC_OP_COPY; // optional
            ColorBlendState.attachmentCount   = static_cast<UInt32>(NumColorBlendAttachments);
            ColorBlendState.pAttachments      = pColorBlendAttachments;
            ColorBlendState.blendConstants[0] = 0.0f; // optional
            ColorBlendState.blendConstants[1] = 0.0f; // optional
            ColorBlendState.blendConstants[2] = 0.0f; // optional
            ColorBlendState.blendConstants[3] = 0.0f; // optional
            return ColorBlendState;
        }

        inline VkGraphicsPipelineCreateInfo GraphicsPipelineCreateInfo(
            VkPipelineShaderStageCreateInfo const        *pShaderStagesInfo,
            SizeT                                         NumShaderStagesInfo,
            VkPipelineVertexInputStateCreateInfo const   &VertexInputStateInfo,
            VkPipelineInputAssemblyStateCreateInfo const &InputAssemblyStateInfo,
            VkPipelineDynamicStateCreateInfo const       &DynamicStateInfo,
            VkPipelineViewportStateCreateInfo const      &ViewportState,
            VkPipelineRasterizationStateCreateInfo const &RasterizerStateInfo,
            VkPipelineMultisampleStateCreateInfo const   &MultisamplerStateInfo,
            VkPipelineDepthStencilStateCreateInfo const  &DepthStencilTestStateInfo,
            VkPipelineColorBlendStateCreateInfo const    &ColorBlendState,
            VkPipelineLayout                              PipelineLayout,
            VkRenderPass                                  RenderPass
        )
        {
            VkGraphicsPipelineCreateInfo PipelineCreateInfo{};
            PipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;

            // Programmable stages
            PipelineCreateInfo.stageCount = static_cast<UInt32>(NumShaderStagesInfo);
            PipelineCreateInfo.pStages    = pShaderStagesInfo;

            // Fixed stages
            PipelineCreateInfo.pVertexInputState   = &VertexInputStateInfo;
            PipelineCreateInfo.pInputAssemblyState = &InputAssemblyStateInfo;
            PipelineCreateInfo.pDynamicState       = &DynamicStateInfo;
            PipelineCreateInfo.pViewportState      = &ViewportState;
            PipelineCreateInfo.pRasterizationState = &RasterizerStateInfo;
            PipelineCreateInfo.pMultisampleState   = &MultisamplerStateInfo;
            PipelineCreateInfo.pDepthStencilState  = &DepthStencilTestStateInfo;
            PipelineCreateInfo.pColorBlendState    = &ColorBlendState;

            // Uniforms and push-constants specified in layout
            PipelineCreateInfo.layout = PipelineLayout;

            // RenderPass and it's Subpass in which Pipeline is used
            PipelineCreateInfo.renderPass = RenderPass;
            PipelineCreateInfo.subpass    = 0;

            PipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
            PipelineCreateInfo.basePipelineIndex  = -1;

            return PipelineCreateInfo;
        }

        inline VkBufferCreateInfo BufferCreateInfo(
            VkBufferUsageFlags Usage, VkDeviceSize Size, VkSharingMode SharingMode
        )
        {
            VkBufferCreateInfo CreateInfo{};
            CreateInfo.sType       = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
            CreateInfo.usage       = Usage;
            CreateInfo.size        = Size;
            CreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE; // only will be used by graphics queue
            return CreateInfo;
        }

        inline VkMemoryAllocateInfo MemoryAllocateInfo(VkDeviceSize Size, UInt32 MemoryTypeIndex)
        {
            VkMemoryAllocateInfo BufferMemoryAllocateInfo{};
            BufferMemoryAllocateInfo.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
            BufferMemoryAllocateInfo.allocationSize  = Size;
            BufferMemoryAllocateInfo.memoryTypeIndex = MemoryTypeIndex;
            return BufferMemoryAllocateInfo;
        }

        inline VkBufferCopy BufferCopy(VkDeviceSize SrcOffset, VkDeviceSize DstOffset, VkDeviceSize Size)
        {
            VkBufferCopy CopyInfo{};
            CopyInfo.srcOffset = SrcOffset;
            CopyInfo.dstOffset = DstOffset;
            CopyInfo.size      = Size;
            return CopyInfo;
        }

        inline VkBufferImageCopy BufferImageCopy(VkExtent3D ImageExtent)
        {
            VkBufferImageCopy Region{};
            Region.bufferOffset                    = 0;
            Region.bufferRowLength                 = 0;
            Region.bufferImageHeight               = 0;
            Region.imageSubresource.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
            Region.imageSubresource.mipLevel       = 0;
            Region.imageSubresource.baseArrayLayer = 0;
            Region.imageSubresource.layerCount     = 1;
            Region.imageOffset                     = VkOffset3D{0, 0, 0};
            Region.imageExtent                     = ImageExtent;
            return Region;
        }

        inline VkImageCreateInfo ImageCreateInfo(
            VkExtent3D        Extent,
            UInt32            MipLevels,
            VkFormat          Format,
            VkImageTiling     Tiling,
            VkImageUsageFlags Usage,
            VkSharingMode     SharingMode
        )
        {
            VkImageCreateInfo CreateInfo{};
            CreateInfo.sType         = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
            CreateInfo.imageType     = VK_IMAGE_TYPE_2D;
            CreateInfo.extent.width  = Extent.width;
            CreateInfo.extent.height = Extent.height;
            CreateInfo.extent.depth  = Extent.depth;
            CreateInfo.mipLevels     = MipLevels;
            CreateInfo.arrayLayers   = 1;
            CreateInfo.format        = Format;
            CreateInfo.tiling        = Tiling;
            CreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            CreateInfo.usage         = Usage;
            CreateInfo.samples       = VK_SAMPLE_COUNT_1_BIT;
            CreateInfo.sharingMode   = SharingMode;
            return CreateInfo;
        }

        inline VkImageBlit ImageBlit(
            Int32 SrcWidth, Int32 SrcHeight, UInt32 SrcMipLevel, Int32 DstWidth, Int32 DstHeight, UInt32 DstMipLevel
        )
        {
            VkImageBlit BlitParams{};
            BlitParams.srcOffsets[0]                 = {0, 0, 0};
            BlitParams.srcOffsets[1]                 = {SrcWidth, SrcHeight, 1};
            BlitParams.srcSubresource.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
            BlitParams.srcSubresource.mipLevel       = SrcMipLevel;
            BlitParams.srcSubresource.baseArrayLayer = 0;
            BlitParams.srcSubresource.layerCount     = 1;
            BlitParams.dstOffsets[0]                 = {0, 0, 0};
            BlitParams.dstOffsets[1]                 = {DstWidth, DstHeight, 1};
            BlitParams.dstSubresource.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
            BlitParams.dstSubresource.mipLevel       = DstMipLevel;
            BlitParams.dstSubresource.baseArrayLayer = 0;
            BlitParams.dstSubresource.layerCount     = 1;
            return BlitParams;
        }

        inline VkImageMemoryBarrier ImageMemoryBarrier(
            VkImage       Image,
            VkImageLayout OldLayout,
            VkImageLayout NewLayout,
            UInt32        BaseMipLevel,
            UInt32        MipLevels,
            VkAccessFlags SrcAccessMask,
            VkAccessFlags DstAccessMask
        )
        {
            VkImageMemoryBarrier Barrier{};
            Barrier.sType                           = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
            Barrier.image                           = Image;
            Barrier.oldLayout                       = OldLayout;
            Barrier.newLayout                       = NewLayout;
            Barrier.srcQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED;
            Barrier.dstQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED;
            Barrier.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
            Barrier.subresourceRange.baseMipLevel   = 0;
            Barrier.subresourceRange.levelCount     = MipLevels;
            Barrier.subresourceRange.baseArrayLayer = 0;
            Barrier.subresourceRange.layerCount     = 1;
            Barrier.srcAccessMask                   = SrcAccessMask;
            Barrier.dstAccessMask                   = DstAccessMask;
            return Barrier;
        }

        inline VkImageViewCreateInfo ImageViewCreateInfo(
            VkImage Image, VkFormat Format, VkImageAspectFlags AspectFlags, UInt32 MipLevels
        )
        {
            VkImageViewCreateInfo CreateInfo{};
            CreateInfo.sType                           = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            CreateInfo.image                           = Image;
            CreateInfo.viewType                        = VK_IMAGE_VIEW_TYPE_2D;
            CreateInfo.format                          = Format;
            CreateInfo.components.r                    = VK_COMPONENT_SWIZZLE_IDENTITY; // Default mapping
            CreateInfo.components.g                    = VK_COMPONENT_SWIZZLE_IDENTITY;
            CreateInfo.components.b                    = VK_COMPONENT_SWIZZLE_IDENTITY;
            CreateInfo.components.a                    = VK_COMPONENT_SWIZZLE_IDENTITY;
            CreateInfo.subresourceRange.aspectMask     = AspectFlags;
            CreateInfo.subresourceRange.baseMipLevel   = 0;
            CreateInfo.subresourceRange.levelCount     = MipLevels;
            CreateInfo.subresourceRange.baseArrayLayer = 0;
            CreateInfo.subresourceRange.layerCount     = 1;
            return CreateInfo;
        }

        inline VkFramebufferCreateInfo FramebufferCreateInfo(
            VkRenderPass       RenderPass,
            VkExtent2D         Extent,
            UInt32             Layers,
            VkImageView const *pAttachmentsViews,
            SizeT              NumAttachmentsViews
        )
        {
            VkFramebufferCreateInfo FramebufferInfo{};
            FramebufferInfo.sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            FramebufferInfo.renderPass      = RenderPass;
            FramebufferInfo.attachmentCount = static_cast<UInt32>(NumAttachmentsViews);
            FramebufferInfo.pAttachments    = pAttachmentsViews;
            FramebufferInfo.width           = Extent.width;
            FramebufferInfo.height          = Extent.height;
            FramebufferInfo.layers          = Layers;
            return FramebufferInfo;
        }

        inline VkSamplerCreateInfo SamplerCreateInfo(
            VkFilter             MinMagFilter,
            VkSamplerAddressMode AddressMode,
            bool                 bAnisoEnabled,
            float                Anisotropy,
            VkSamplerMipmapMode  Filtering,
            UInt32               MipLevels
        )
        {
            VkSamplerCreateInfo SamplerCreateInfo{};
            SamplerCreateInfo.sType                   = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
            SamplerCreateInfo.minFilter               = MinMagFilter;
            SamplerCreateInfo.magFilter               = MinMagFilter;
            SamplerCreateInfo.addressModeU            = AddressMode;
            SamplerCreateInfo.addressModeV            = AddressMode;
            SamplerCreateInfo.addressModeW            = AddressMode;
            SamplerCreateInfo.anisotropyEnable        = bAnisoEnabled ? VK_TRUE : VK_FALSE;
            SamplerCreateInfo.maxAnisotropy           = Anisotropy;
            SamplerCreateInfo.borderColor             = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
            SamplerCreateInfo.unnormalizedCoordinates = VK_FALSE;
            SamplerCreateInfo.compareEnable           = VK_FALSE;
            SamplerCreateInfo.compareOp               = VK_COMPARE_OP_ALWAYS;
            SamplerCreateInfo.mipmapMode              = Filtering;
            SamplerCreateInfo.mipLodBias              = 0.0f;
            SamplerCreateInfo.minLod                  = 0.0f;
            SamplerCreateInfo.maxLod                  = static_cast<float>(MipLevels);
            return SamplerCreateInfo;
        }

        inline VkVertexInputBindingDescription VertexInputBindingDescription(UInt32 Stride, VkVertexInputRate InputRate)
        {
            VkVertexInputBindingDescription InputBindingDescription{};
            InputBindingDescription.binding   = 0;
            InputBindingDescription.stride    = Stride;
            InputBindingDescription.inputRate = InputRate;
            return InputBindingDescription;
        }

        inline VkVertexInputAttributeDescription VertexInputAttributeDescription(
            UInt32 Binding, UInt32 Location, VkFormat Format, UInt32 Offset
        )
        {
            VkVertexInputAttributeDescription Attribute{};
            Attribute.binding  = Binding;
            Attribute.location = Location;
            Attribute.format   = Format;
            Attribute.offset   = Offset;
            return Attribute;
        }

        inline VkSemaphoreCreateInfo SemaphoreCreateInfo(VkSemaphoreCreateFlags Flags)
        {
            VkSemaphoreCreateInfo CreateInfo{};
            CreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
            CreateInfo.flags = Flags;
            return CreateInfo;
        }

        inline VkFenceCreateInfo FenceCreateInfo(VkFenceCreateFlags Flags)
        {
            VkFenceCreateInfo CreateInfo{};
            CreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
            CreateInfo.flags = Flags;
            return CreateInfo;
        }

        inline VkPresentInfoKHR PresentInfo(
            VkSwapchainKHR const *pSwapchains,
            SizeT                 NumSwapchains,
            UInt32 const         *pSwapchainImagesIndices,
            VkSemaphore const    *pWaitSemaphores,
            SizeT                 NumSemaphores
        )
        {
            VkPresentInfoKHR PresentInfo{};
            PresentInfo.sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
            PresentInfo.swapchainCount     = static_cast<UInt32>(NumSwapchains);
            PresentInfo.pSwapchains        = pSwapchains;
            PresentInfo.pImageIndices      = pSwapchainImagesIndices;
            PresentInfo.waitSemaphoreCount = static_cast<UInt32>(NumSemaphores);
            PresentInfo.pWaitSemaphores    = pWaitSemaphores;
            PresentInfo.pResults           = nullptr;
            return PresentInfo;
        }

    } // namespace VkInit

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_RENDERER_INITIALIZERS_H
