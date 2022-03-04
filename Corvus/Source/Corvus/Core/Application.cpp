#include "CorvusPCH.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Corvus/Core/Application.h"

namespace Corvus 
{

    Application::Application()
    {

    }

    Application::~Application()
    {

    }

    void Application::Run()
    {
        CORVUS_CORE_TRACE(STR("Running the application!"));

        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        GLFWwindow *window = glfwCreateWindow(300, 200, "Hello, Window", NULL, NULL);
        if (!window) {
            glfwTerminate();
            return;
        }

        glfwMakeContextCurrent(window);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            return;
        }

        glViewport(0, 0, 300, 200);

        while (!glfwWindowShouldClose(window)) {

            if (glfwGetKey(window, GLFW_KEY_BACKSPACE) == GLFW_PRESS) {
                glfwSetWindowShouldClose(window, true);
            }

            glClearColor(0.3f, 0.15f, 0.6f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            // Swap front and back buffers
            glfwSwapBuffers(window);

            // Process pending events
            glfwPollEvents();
        }

        glfwTerminate();

        //while (1);

        CORVUS_CORE_TRACE(STR("Application finished running!"));
    }

}