#include "CorvusPCH.h"

#include "Corvus/Renderer/Renderer.h"

#include "Corvus/Assets/Model/StaticModel.h"
#include "Corvus/Camera/Camera.h"
#include "Corvus/Core/Application.h"

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

    VkInstance CRenderer::GetVulkanInstance()
    {
        return m_Instance;
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

        CreateDescriptorSetLayout();
        CreateDescriptorPools();
        AllocateDescriptorSets();

        CreateRenderPass();
        CreateGUIRenderPass();

        CreatePipelineLayout();
        CreatePipeline();

        CreateFramebuffers();

        CreateSyncObjects();
    }

    void CRenderer::Destroy()
    {
        DestroySyncObjects();

        DestroyFramebuffers();

        DestroyPipeline();
        DestroyPipelineLayout();

        DestroyGUIRenderPass();
        DestroyRenderPass();

        DestroyDescriptorPools();
        DestroyDescriptorSetLayout();

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
        vkWaitForFences(m_Device, 1, &m_InFlightFences[m_CurrentFrame], VK_TRUE, UINT64_MAX);

        // Acquire an image from the swap chain
        VkResult AcquisitionResult = GetNextSwapchainImageIndex(m_SwapchainImageIndex);
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
        vkResetFences(m_Device, 1, &m_InFlightFences[m_CurrentFrame]);

        // Start recording a command buffer which draws the scene onto that image
        VkCommandBuffer CommandBuffer = m_CommandBuffers[m_CurrentFrame];
        vkResetCommandBuffer(CommandBuffer, 0);

        VkCommandBufferBeginInfo CommandBufferBeginInfo{};
        CommandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(CommandBuffer, &CommandBufferBeginInfo) != VK_SUCCESS)
        {
            CORVUS_CORE_CRITICAL("Failed to Begin Vulkan Command Buffer!");
        }

        VkRenderPassBeginInfo RenderPassBeginInfo{};
        RenderPassBeginInfo.sType             = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        RenderPassBeginInfo.renderPass        = m_RenderPass;
        RenderPassBeginInfo.framebuffer       = m_SwapchainFramebuffers[m_SwapchainImageIndex];
        RenderPassBeginInfo.renderArea.offset = {0, 0};
        RenderPassBeginInfo.renderArea.extent = m_SwapchainExtent;

        std::array<VkClearValue, 2> ClearColors;
        ClearColors[0].color        = VkClearColorValue{0.6f, 0.8f, 1.0f, 1.0f};
        ClearColors[1].depthStencil = VkClearDepthStencilValue{1.0f, 0};

        RenderPassBeginInfo.clearValueCount = static_cast<UInt32>(ClearColors.size());
        RenderPassBeginInfo.pClearValues    = ClearColors.data();

        vkCmdBeginRenderPass(CommandBuffer, &RenderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(CommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_Pipeline);

        // Viewport and Scissor are dynamic - specify them here
        VkViewport Viewport{};
        Viewport.x        = 0.0f;
        Viewport.y        = 0.0f;
        Viewport.width    = static_cast<float>(m_SwapchainExtent.width);
        Viewport.height   = static_cast<float>(m_SwapchainExtent.height);
        Viewport.minDepth = 0.0f;
        Viewport.maxDepth = 1.0f;
        vkCmdSetViewport(CommandBuffer, 0, 1, &Viewport);

        VkRect2D Scissor{};
        Scissor.offset = {0, 0};
        Scissor.extent = m_SwapchainExtent;
        vkCmdSetScissor(CommandBuffer, 0, 1, &Scissor);

        vkCmdBindDescriptorSets(
            CommandBuffer,
            VK_PIPELINE_BIND_POINT_GRAPHICS,
            m_PipelineLayout,
            0,
            1,
            &m_DescriptorSets[m_CurrentFrame],
            0,
            nullptr
        );
    }

    void CRenderer::EndFrame()
    {
        VkCommandBuffer CommandBuffer = m_CommandBuffers[m_CurrentFrame];
        vkCmdEndRenderPass(CommandBuffer);

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

        VkCommandBuffer CommandBuffer = m_CommandBuffers[m_CurrentFrame];

        for (CStaticMesh &StaticMesh : StaticModel)
        {
            for (CStaticMeshPrimitive &Primitive : StaticMesh)
            {
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
        vkDeviceWaitIdle(m_Device);
    }

    void CRenderer::SetModelMatrix(FMatrix4 const &ModelMatrix)
    {
        UInt8 *MVPUBOStart         = static_cast<UInt8 *>(m_MatricesUBOs[m_CurrentFrame].MappedMemory);
        UInt8 *ModelMatrixLocation = MVPUBOStart + offsetof(CMVPUBO, Model);
        std::memcpy(ModelMatrixLocation, &ModelMatrix, sizeof(ModelMatrix));
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

        UInt8 *MVPUBOStart          = static_cast<UInt8 *>(m_MatricesUBOs[m_CurrentFrame].MappedMemory);
        UInt8 *CameraMatrixLocation = MVPUBOStart + offsetof(CMVPUBO, ProjectionView);

        std::memcpy(CameraMatrixLocation, &CameraProjectionView, sizeof(CameraProjectionView));
    }

    VkResult CRenderer::GetNextSwapchainImageIndex(UInt32 &ImageIndex)
    {
        return vkAcquireNextImageKHR(
            m_Device, m_Swapchain, UINT64_MAX, m_ImageAvailableSemaphores[m_CurrentFrame], VK_NULL_HANDLE, &ImageIndex
        );
    }

    void CRenderer::SubmitCommandBuffer(VkCommandBuffer CommandBuffer)
    {
        VkSemaphore          WaitSemaphores[] = {m_ImageAvailableSemaphores[m_CurrentFrame]};
        VkPipelineStageFlags WaitStages[]     = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};

        VkSemaphore SignalSemaphores[] = {m_RenderFinishedSemaphores[m_CurrentFrame]};

        VkSubmitInfo SubmitInfo{};
        SubmitInfo.sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        SubmitInfo.waitSemaphoreCount   = 1;
        SubmitInfo.pWaitSemaphores      = WaitSemaphores;
        SubmitInfo.pWaitDstStageMask    = WaitStages;
        SubmitInfo.commandBufferCount   = 1;
        SubmitInfo.pCommandBuffers      = &CommandBuffer;
        SubmitInfo.signalSemaphoreCount = 1;
        SubmitInfo.pSignalSemaphores    = SignalSemaphores;

        if (vkQueueSubmit(m_Queues.GraphicsQueue, 1, &SubmitInfo, m_InFlightFences[m_CurrentFrame]) != VK_SUCCESS)
        {
            CORVUS_CORE_CRITICAL("Failed to submit draw Commands Buffer!");
        }
    }

    VkResult CRenderer::PresentResult()
    {
        VkSwapchainKHR Swapchains[] = {m_Swapchain};

        VkSemaphore WaitSemaphores[] = {m_RenderFinishedSemaphores[m_CurrentFrame]};

        VkPresentInfoKHR PresentInfo{};
        PresentInfo.sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        PresentInfo.waitSemaphoreCount = 1;
        PresentInfo.pWaitSemaphores    = WaitSemaphores;
        PresentInfo.swapchainCount     = 1;
        PresentInfo.pSwapchains        = Swapchains;
        PresentInfo.pImageIndices      = &m_SwapchainImageIndex;
        PresentInfo.pResults           = nullptr;

        return vkQueuePresentKHR(m_Queues.GraphicsQueue, &PresentInfo);
    }

} // namespace Corvus
