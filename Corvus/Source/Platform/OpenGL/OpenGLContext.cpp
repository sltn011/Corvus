#include "CorvusPCH.h"
#include "Platform/OpenGL/OpenGLContext.h"

#include "Corvus/Window/Window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Corvus
{
    OpenGLContext::OpenGLContext(Window &OwnerWindow)
        : m_WindowHandle{ static_cast<GLFWwindow*>(OwnerWindow.GetRawWindow()) }
    {
        CORVUS_ASSERT(m_WindowHandle);
    }

    void OpenGLContext::Init()
    {
        glfwMakeContextCurrent(m_WindowHandle);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            CORVUS_NO_ENTRY_FMT("Failed to initialize rendering context!");
        }
        CORVUS_CORE_TRACE("GLAD initialized");

        CORVUS_CORE_TRACE("Vendor: {0}", glGetString(GL_VENDOR));
        CORVUS_CORE_TRACE("Renderer: {0}", glGetString(GL_RENDERER));
        CORVUS_CORE_TRACE("Version: {0}", glGetString(GL_VERSION));

        CORVUS_CORE_ASSERT_FMT(GLVersion.major >= 4 && GLVersion.minor >= 6, "OpenGL version 4.6 minimum required!");
    }

    void OpenGLContext::SwapBuffers()
    {
        glfwSwapBuffers(m_WindowHandle);
    }

}
