#include "CorvusPCH.h"

#include "Corvus/Core/Application.h"
#include "Corvus/Core/Input.h"

#include <GLFW/glfw3.h>

namespace Corvus
{
    bool Input::IsKeyInState(KeyCode const Key, ActionCode const State)
    {
        Application      &App    = Application::GetInstance();
        GLFWwindow *const Window = static_cast<GLFWwindow *>(App.GetWindow().GetRawWindow());
        return glfwGetKey(Window, static_cast<int>(Key)) == State;
    }

    bool Input::IsKeyPressed(KeyCode const Key)
    {
        return IsKeyInState(Key, Action::Press);
    }

    bool Input::IsKeyReleased(KeyCode const Key)
    {
        return IsKeyInState(Key, Action::Release);
    }

    bool Input::IsKeyRepeated(KeyCode const Key)
    {
        return IsKeyInState(Key, Action::Repeat);
    }

    bool Input::IsButtonInState(MouseCode const Button, ActionCode const State)
    {
        Application      &App    = Application::GetInstance();
        GLFWwindow *const Window = static_cast<GLFWwindow *>(App.GetWindow().GetRawWindow());
        return glfwGetMouseButton(Window, static_cast<int>(Button)) == State;
    }

    bool Input::IsButtonPressed(MouseCode const Button)
    {
        return IsButtonInState(Button, Action::Press);
    }

    bool Input::IsButtonReleased(MouseCode const Button)
    {
        return IsButtonInState(Button, Action::Release);
    }

    void Input::SetCursorEnabled(bool const bEnabled)
    {
        Application      &App    = Application::GetInstance();
        GLFWwindow *const Window = static_cast<GLFWwindow *>(App.GetWindow().GetRawWindow());

        glfwSetInputMode(Window, GLFW_CURSOR, bEnabled ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
    }

    Vec2 Input::GetCursorPos()
    {
        Application      &App    = Application::GetInstance();
        GLFWwindow *const Window = static_cast<GLFWwindow *>(App.GetWindow().GetRawWindow());

        double XPos, YPos;
        glfwGetCursorPos(Window, &XPos, &YPos);

        return Vec2{static_cast<float>(XPos), static_cast<float>(YPos)};
    }

    float Input::GetCursorX()
    {
        return GetCursorPos().x;
    }

    float Input::GetCursorY()
    {
        return GetCursorPos().y;
    }

    void Input::SetCursorPos(Vec2 const &Pos)
    {
        Application      &App    = Application::GetInstance();
        GLFWwindow *const Window = static_cast<GLFWwindow *>(App.GetWindow().GetRawWindow());

        glfwSetCursorPos(Window, Pos.x, Pos.y);
    }
} // namespace Corvus