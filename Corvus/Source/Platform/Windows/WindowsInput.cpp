#include "CorvusPCH.h"
#include "Corvus/Core/Input.h"

#include "Corvus/Core/Application.h"

#include <GLFW/glfw3.h>

namespace Corvus
{
    bool Input::IsKeyInState(KeyCode Key, ActionCode State)
    {
        Application &App = Application::GetInstance();
        GLFWwindow *Window = static_cast<GLFWwindow *>(App.GetWindow().GetRawWindow());
        return glfwGetKey(Window, static_cast<int>(Key)) == State;
    }

    bool Input::IsKeyPressed(KeyCode Key)
    {
        return IsKeyInState(Key, Action::Press);
    }

    bool Input::IsKeyReleased(KeyCode Key)
    {
        return IsKeyInState(Key, Action::Release);
    }

    bool Input::IsKeyRepeated(KeyCode Key)
    {
        return IsKeyInState(Key, Action::Repeat);
    }

    bool Input::IsButtonInState(MouseCode Button, ActionCode State)
    {
        Application &App = Application::GetInstance();
        GLFWwindow *Window = static_cast<GLFWwindow *>(App.GetWindow().GetRawWindow());
        return glfwGetMouseButton(Window, static_cast<int>(Button)) == State;
    }

    bool Input::IsButtonPressed(MouseCode Button)
    {
        return IsButtonInState(Button, Action::Press);
    }

    bool Input::IsButtonReleased(MouseCode Button)
    {
        return IsButtonInState(Button, Action::Release);
    }

    glm::vec2 Input::GetCursorPos()
    {
        Application &App = Application::GetInstance();
        GLFWwindow *Window = static_cast<GLFWwindow *>(App.GetWindow().GetRawWindow());

        double XPos, YPos;
        glfwGetCursorPos(Window, &XPos, &YPos);

        return glm::vec2{ static_cast<float>(XPos), static_cast<float>(YPos) };
    }

    float Input::GetCursorX()
    {
        return GetCursorPos().x;
    }

    float Input::GetCursorY()
    {
        return GetCursorPos().y;
    }
}