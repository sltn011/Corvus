#include "CorvusPCH.h"

#include "Corvus/Renderer/Renderer.h"

#include "Corvus/Assets/Model/StaticModel.h"
#include "Corvus/Assets/Texture/Texture2D.h"
#include "Corvus/Camera/Camera.h"
#include "Corvus/Core/Application.h"
#include "Corvus/Renderer/Data/PushConstants.h"
#include "Corvus/Renderer/Data/ScreenQuad.h"

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

        CreateDepthResources();

        CreateUniformBuffers();

        CreateSamplers();

        CreateDescriptorSetLayout();
        CreateDescriptorPools();
        CreatePerFrameDescriptorSets();

        CreateRenderPass();
        CreatePipelineLayout();
        CreatePipeline();

        RenderPass_Deferred.Create();
        RenderPass_Combine.Create();

        CreateFramebuffers();

        CreateSyncObjects();

        CScreenQuad::Get().Create();
    }

    void CRenderer::Destroy()
    {
        CScreenQuad::Get().Destroy();

        DestroySyncObjects();

        DestroyFramebuffers();

        RenderPass_Combine.Destroy();
        RenderPass_Deferred.Destroy();

        DestroyPipeline();
        DestroyPipelineLayout();
        DestroyRenderPass();

        DestroyDescriptorPools();
        DestroyDescriptorSetLayout();

        DestroySamplers();

        DestroyUniformBuffers();

        DestroyDepthResources();

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
        SetCameraMatrices();

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

        VkCommandBufferBeginInfo CommandBufferBeginInfo{};
        CommandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

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

        RenderPass_Combine.Render(CommandBuffer, SwapchainImageIndex);

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
        // UInt8 *MVPUBOStart         = static_cast<UInt8 *>(MatricesUBOs[m_CurrentFrame].MappedMemory);
        // UInt8 *ModelMatrixLocation = MVPUBOStart + offsetof(CMVPUBO, Model);
        // std::memcpy(ModelMatrixLocation, &ModelMatrix, sizeof(ModelMatrix));

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

    void CRenderer::SetCameraMatrices()
    {
        CCamera *PlayerCamera = CApplication::GetInstance().Scene.GetPlayerCamera();
        CORVUS_CORE_ASSERT(PlayerCamera != nullptr);

        FMatrix4 CameraView       = PlayerCamera->GetViewMatrix();
        FMatrix4 CameraProjection = PlayerCamera->GetProjectionMatrix();

        // GLM uses OpenGL's clip coordinates, flip Y to fix
        // But this flips CW and CCW polygon rotation
        CameraProjection[1][1] *= -1;

        FMatrix4 CameraProjectionView = CameraProjection * CameraView;

        UInt8 *VPUBOStart           = static_cast<UInt8 *>(MatricesUBOs[m_CurrentFrame].MappedMemory);
        UInt8 *CameraMatrixLocation = VPUBOStart + offsetof(CVPUBO, ProjectionView);

        std::memcpy(CameraMatrixLocation, &CameraProjectionView, sizeof(CameraProjectionView));
    }

    VkResult CRenderer::GetNextSwapchainImageIndex(UInt32 &ImageIndex)
    {
        return vkAcquireNextImageKHR(
            Device, Swapchain, UINT64_MAX, ImageAvailableSemaphores[m_CurrentFrame], VK_NULL_HANDLE, &ImageIndex
        );
    }

    void CRenderer::SubmitCommandBuffer(VkCommandBuffer CommandBuffer)
    {
        VkSemaphore          WaitSemaphores[] = {ImageAvailableSemaphores[m_CurrentFrame]};
        VkPipelineStageFlags WaitStages[]     = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};

        VkSemaphore SignalSemaphores[] = {RenderFinishedSemaphores[m_CurrentFrame]};

        VkSubmitInfo SubmitInfo{};
        SubmitInfo.sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        SubmitInfo.waitSemaphoreCount   = 1;
        SubmitInfo.pWaitSemaphores      = WaitSemaphores;
        SubmitInfo.pWaitDstStageMask    = WaitStages;
        SubmitInfo.commandBufferCount   = 1;
        SubmitInfo.pCommandBuffers      = &CommandBuffer;
        SubmitInfo.signalSemaphoreCount = 1;
        SubmitInfo.pSignalSemaphores    = SignalSemaphores;

        if (vkQueueSubmit(Queues.GraphicsQueue, 1, &SubmitInfo, InFlightFences[m_CurrentFrame]) != VK_SUCCESS)
        {
            CORVUS_CORE_CRITICAL("Failed to submit draw Commands Buffer!");
        }
    }

    VkResult CRenderer::PresentResult()
    {
        VkSwapchainKHR Swapchains[] = {Swapchain};

        VkSemaphore WaitSemaphores[] = {RenderFinishedSemaphores[m_CurrentFrame]};

        VkPresentInfoKHR PresentInfo{};
        PresentInfo.sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        PresentInfo.waitSemaphoreCount = 1;
        PresentInfo.pWaitSemaphores    = WaitSemaphores;
        PresentInfo.swapchainCount     = 1;
        PresentInfo.pSwapchains        = Swapchains;
        PresentInfo.pImageIndices      = &SwapchainImageIndex;
        PresentInfo.pResults           = nullptr;

        return vkQueuePresentKHR(Queues.GraphicsQueue, &PresentInfo);
    }

} // namespace Corvus
