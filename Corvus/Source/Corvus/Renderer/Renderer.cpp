#include "CorvusPCH.h"

#include "Corvus/Renderer/Renderer.h"

#include "Corvus/Assets/Model/StaticModel.h"
#include "Corvus/Assets/Texture/Texture2D.h"
#include "Corvus/Camera/Camera.h"
#include "Corvus/Core/Application.h"
#include "Corvus/Renderer/Data/PushConstants.h"

namespace Corvus
{
    bool CRenderer::s_bInitialized = false;

    CRenderer &Renderer()
    {
        return CRenderer::GetInstance();
    }

    CRenderer &CRenderer::GetInstance()
    {
        static CRenderer Renderer;
        return Renderer;
    }

    void CRenderer::Create()
    {
        CORVUS_ASSERT_FMT(!s_bInitialized, "Renderer was already initialized!");

        s_bInitialized = true;

        CreateInstance(CApplication::GetInstance().GetWindow().GetWindowName(), VK_API_VERSION_1_3);

#ifdef CORVUS_DEBUG
        CreateDebugCallback();
#endif // CORVUS_DEBUG

        CreateSurface();

        SelectPhysicalDevice();
        CreateDevice();
        RetrieveQueues();

        CreateSwapchain();
        RetrieveSwapchainImages();
        CreateSwapchainImageViews();

        CreateCommandPools();
        AllocateCommandBuffers();

        CreateDescriptorSetLayout();
        CreateDescriptorPools();

        CreateSamplers();

        CreateDepthResources();

        CreateUniformBuffers();

        CreatePerFrameDescriptorSets();

        CreateRenderPass();
        CreatePipelineLayout();
        CreatePipeline();

        RenderPass_Deferred.Create();
        RenderPass_Combine.Create();
        RenderPass_Postprocess.Create();

        CreateFramebuffers();

        CreateSyncObjects();

        ScreenQuad.Create();
    }

    void CRenderer::Destroy()
    {
        ScreenQuad.Destroy();

        DestroySyncObjects();

        DestroyFramebuffers();

        RenderPass_Postprocess.Destroy();
        RenderPass_Combine.Destroy();
        RenderPass_Deferred.Destroy();

        DestroyPipeline();
        DestroyPipelineLayout();
        DestroyRenderPass();

        DestroyUniformBuffers();

        DestroyDepthResources();

        DestroySamplers();

        DestroyDescriptorPools();
        DestroyDescriptorSetLayout();

        DestroyCommandPools();

        DestroySwapchainImageViews();
        DestroySwapchain();

        DestroyDevice();

        DestroySurface();

#ifdef CORVUS_DEBUG
        DestroyDebugCallback();
#endif // CORVUS_DEBUG

        DestroyInstance();

        s_bInitialized = false;
    }

    CRenderer::~CRenderer()
    {
        CORVUS_ASSERT_FMT(!s_bInitialized, "Renderer was not destroyed properly!");
    }

    void CRenderer::BeginFrame()
    {
        SetFrameUniforms();

        // Wait for the previous frame to finish
        vkWaitForFences(Device, 1, &InFlightFences[m_CurrentFrame], VK_TRUE, UINT64_MAX);

        // Acquire an image from the swap chain
        VkResult AcquisitionResult = GetNextSwapchainImageIndex(SwapchainImageIndex);
        if (AcquisitionResult == VK_ERROR_OUT_OF_DATE_KHR) // swapchain not suitable now(after screen resize)
        {
            RecreateSwapchain();
            return;
        }
        else if (AcquisitionResult != VK_SUCCESS)
        {
            if (AcquisitionResult != VK_SUBOPTIMAL_KHR) // Swapchain properties not matched exactly
            {
                CORVUS_CORE_CRITICAL("Failed to acquire Swapchain image!");
            }
        }

        // Reset fences here to avoid deadlock
        vkResetFences(Device, 1, &InFlightFences[m_CurrentFrame]);

        // Start recording a command buffer which draws the scene onto that image
        VkCommandBuffer CommandBuffer = CommandBuffers[m_CurrentFrame];
        vkResetCommandBuffer(CommandBuffer, 0);

        VkCommandBufferBeginInfo CommandBufferBeginInfo = VkInit::CommandBufferBeginInfo(0);

        if (vkBeginCommandBuffer(CommandBuffer, &CommandBufferBeginInfo) != VK_SUCCESS)
        {
            CORVUS_CORE_CRITICAL("Failed to Begin Vulkan Command Buffer!");
        }

        RenderPass_Deferred.BeginRender(CommandBuffer);
    }

    void CRenderer::EndFrame()
    {
        VkCommandBuffer CommandBuffer = CommandBuffers[m_CurrentFrame];

        RenderPass_Deferred.EndRender(CommandBuffer);

        RenderPass_Combine.Render(CommandBuffer);

        RenderPass_Postprocess.Render(CommandBuffer);

        if (vkEndCommandBuffer(CommandBuffer) != VK_SUCCESS)
        {
            CORVUS_CORE_CRITICAL("Failed to End Vulkan Command Buffer!");
        }

        // Submit recorded Command Buffer
        SubmitCommandBuffer(CommandBuffer);

        // Present the swap chain image
        VkResult QueuePresentResult = PresentResult();
        if (QueuePresentResult == VK_ERROR_OUT_OF_DATE_KHR || QueuePresentResult == VK_SUBOPTIMAL_KHR ||
            m_bWindowResizeHappened)
        {
            RecreateSwapchain();
            m_bWindowResizeHappened = false;
        }
        else if (QueuePresentResult != VK_SUCCESS)
        {
            CORVUS_CORE_CRITICAL("Failed to present Swapchain Image!");
        }

        m_CurrentFrame = (m_CurrentFrame + 1) % s_FramesInFlight;
    }

    void CRenderer::SubmitStaticModel(CStaticModel &StaticModel, FMatrix4 const &ModelTransformMatrix)
    {
        SetModelMatrix(ModelTransformMatrix);

        VkCommandBuffer CommandBuffer = CommandBuffers[m_CurrentFrame];

        for (CStaticMesh &StaticMesh : StaticModel)
        {
            for (CStaticMeshPrimitive &Primitive : StaticMesh)
            {
                vkCmdBindDescriptorSets(
                    CommandBuffer,
                    VK_PIPELINE_BIND_POINT_GRAPHICS,
                    PipelineLayout,
                    1,
                    1,
                    &Primitive.Material.DescriptorSet,
                    0,
                    nullptr
                );

                VkBuffer     Buffers[] = {Primitive.VertexBuffer.Buffer};
                VkDeviceSize Offsets[] = {0};
                vkCmdBindVertexBuffers(CommandBuffer, 0, 1, Buffers, Offsets);

                vkCmdBindIndexBuffer(CommandBuffer, Primitive.IndexBuffer.Buffer, 0, VK_INDEX_TYPE_UINT16);

                vkCmdDrawIndexed(CommandBuffer, static_cast<UInt32>(Primitive.IndexData.size()), 1, 0, 0, 0);
            }
        }
    }

    void CRenderer::NotifyWindowResize()
    {
        m_bWindowResizeHappened = true;
    }

    void CRenderer::AwaitIdle()
    {
        vkDeviceWaitIdle(Device);
    }

    void CRenderer::SetModelMatrix(FMatrix4 const &ModelMatrix)
    {
        CModelPushConstant PushConstant{};
        PushConstant.Model = ModelMatrix;

        vkCmdPushConstants(
            CommandBuffers[m_CurrentFrame],
            PipelineLayout,
            VK_SHADER_STAGE_VERTEX_BIT,
            0,
            static_cast<UInt32>(sizeof(PushConstant)),
            &PushConstant
        );
    }

    void CRenderer::SetFrameUniforms()
    {
        // Camera
        {
            UInt8 *CameraUBOStart = static_cast<UInt8 *>(CameraUBOs[m_CurrentFrame].MappedMemory);

            CCamera *PlayerCamera = CApplication::GetInstance().Scene.GetPlayerCamera();
            CORVUS_CORE_ASSERT(PlayerCamera != nullptr);

            FMatrix4 CameraView       = PlayerCamera->GetViewMatrix();
            FMatrix4 CameraProjection = PlayerCamera->GetProjectionMatrix();

            // GLM uses OpenGL's clip coordinates, flip Y to fix
            // But this flips CW and CCW polygon rotation
            CameraProjection[1][1] *= -1;

            CCameraUBO CameraUniformsData;
            CameraUniformsData.ProjectionView = CameraProjection * CameraView;

            UInt8 *CameraUniformsLocation = CameraUBOStart + offsetof(CCameraUBO, ProjectionView);
            std::memcpy(CameraUniformsLocation, &CameraUniformsData, sizeof(CameraUniformsData));
        }

        // RT
        {
            UInt8 *RenderTargetUBOStart = static_cast<UInt8 *>(RenderTargetUBOs[m_CurrentFrame].MappedMemory);

            CRenderTargetUBO RTUniformsData;
            RTUniformsData.RTFullSize = FVector4{SwapchainExtent.width, SwapchainExtent.height, 0, 0};

            UInt8 *RTUniformsLocation = RenderTargetUBOStart + offsetof(CRenderTargetUBO, RTFullSize);
            std::memcpy(RTUniformsLocation, &RTUniformsData, sizeof(RTUniformsData));
        }
    }

    VkResult CRenderer::GetNextSwapchainImageIndex(UInt32 &ImageIndex)
    {
        return vkAcquireNextImageKHR(
            Device, Swapchain, UINT64_MAX, ImageAvailableSemaphores[m_CurrentFrame], VK_NULL_HANDLE, &ImageIndex
        );
    }

    void CRenderer::SubmitCommandBuffer(VkCommandBuffer CommandBuffer)
    {
        std::vector<VkSemaphore>          WaitSemaphores   = {ImageAvailableSemaphores[m_CurrentFrame]};
        std::vector<VkPipelineStageFlags> WaitStages       = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        std::vector<VkSemaphore>          SignalSemaphores = {RenderFinishedSemaphores[m_CurrentFrame]};

        VkSubmitInfo SubmitInfo = VkInit::SubmitInfo(
            CommandBuffer,
            WaitSemaphores.data(),
            WaitSemaphores.size(),
            WaitStages.data(),
            SignalSemaphores.data(),
            SignalSemaphores.size()
        );

        if (vkQueueSubmit(Queues.GraphicsQueue, 1, &SubmitInfo, InFlightFences[m_CurrentFrame]) != VK_SUCCESS)
        {
            CORVUS_CORE_CRITICAL("Failed to submit draw Commands Buffer!");
        }
    }

    VkResult CRenderer::PresentResult()
    {
        VkSwapchainKHR Swapchains[]             = {Swapchain};
        UInt32         SwapchainImagesIndices[] = {SwapchainImageIndex};
        VkSemaphore    WaitSemaphores[]         = {RenderFinishedSemaphores[m_CurrentFrame]};

        VkPresentInfoKHR PresentInfo = VkInit::PresentInfo(Swapchains, 1, SwapchainImagesIndices, WaitSemaphores, 1);

        return vkQueuePresentKHR(Queues.GraphicsQueue, &PresentInfo);
    }

} // namespace Corvus
