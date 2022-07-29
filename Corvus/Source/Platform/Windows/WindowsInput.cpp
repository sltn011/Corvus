#include "CorvusPCH.h"
#include "Corvus/Core/Input.h"

#include "Corvus/Core/Application.h"

#include <GLFW/glfw3.h>

namespace Corvus
{
    bool Input::IsKeyInState(KeyCode Key, ActionCode State)
    {
        Application &App = Application::GetInstance();
        GLFWwindow  *Window = static_cast<GLFWwindow *>(App.GetWindow().GetRawWindow());
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
        GLFWwindow  *Window = static_cast<GLFWwindow *>(App.GetWindow().GetRawWindow());
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

    void Input::SetCursorEnabled(bool bEnabled)
    {
        Application &App = Application::GetInstance();
        GLFWwindow  *Window = static_cast<GLFWwindow *>(App.GetWindow().GetRawWindow());

        glfwSetInputMode(Window, GLFW_CURSOR, bEnabled ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
    }

    Vec2 Input::GetCursorPos()
    {
        Application &App = Application::GetInstance();
        GLFWwindow  *Window = static_cast<GLFWwindow *>(App.GetWindow().GetRawWindow());

        double XPos, YPos;
        glfwGetCursorPos(Window, &XPos, &YPos);

        return Vec2{ static_cast<float>(XPos), static_cast<float>(YPos) };
    }

    float Input::GetCursorX()
    {
        return GetCursorPos().x;
    }

    float Input::GetCursorY()
    {
        return GetCursorPos().y;
    }

    void Input::SetCursorPos(Vec2 Pos)
    {
        Application &App = Application::GetInstance();
        GLFWwindow  *Window = static_cast<GLFWwindow *>(App.GetWindow().GetRawWindow());

        glfwSetCursorPos(Window, Pos.x, Pos.y);
    }
}