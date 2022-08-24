#include "CorvusPCH.h"

#include "Corvus/Core/Application.h"
#include "Corvus/Core/Input.h"

#include <GLFW/glfw3.h>

namespace Corvus
{
    bool СInput::IsKeyInState(EKeyCode const Key, EActionCode const State)
    {
        СApplication     &App    = СApplication::GetInstance();
        GLFWwindow *const Window = static_cast<GLFWwindow *>(App.GetWindow().GetRawWindow());
        return glfwGetKey(Window, static_cast<int>(Key)) == State;
    }

    bool СInput::IsKeyPressed(EKeyCode const Key)
    {
        return IsKeyInState(Key, Action::Press);
    }

    bool СInput::IsKeyReleased(EKeyCode const Key)
    {
        return IsKeyInState(Key, Action::Release);
    }

    bool СInput::IsKeyRepeated(EKeyCode const Key)
    {
        return IsKeyInState(Key, Action::Repeat);
    }

    bool СInput::IsButtonInState(EMouseCode const Button, EActionCode const State)
    {
        СApplication     &App    = СApplication::GetInstance();
        GLFWwindow *const Window = static_cast<GLFWwindow *>(App.GetWindow().GetRawWindow());
        return glfwGetMouseButton(Window, static_cast<int>(Button)) == State;
    }

    bool СInput::IsButtonPressed(EMouseCode const Button)
    {
        return IsButtonInState(Button, Action::Press);
    }

    bool СInput::IsButtonReleased(EMouseCode const Button)
    {
        return IsButtonInState(Button, Action::Release);
    }

    void СInput::SetCursorEnabled(bool const bEnabled)
    {
        СApplication     &App    = СApplication::GetInstance();
        GLFWwindow *const Window = static_cast<GLFWwindow *>(App.GetWindow().GetRawWindow());

        glfwSetInputMode(Window, GLFW_CURSOR, bEnabled ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
    }

    FVector2 СInput::GetCursorPos()
    {
        СApplication     &App    = СApplication::GetInstance();
        GLFWwindow *const Window = static_cast<GLFWwindow *>(App.GetWindow().GetRawWindow());

        double XPos, YPos;
        glfwGetCursorPos(Window, &XPos, &YPos);

        return FVector2{static_cast<float>(XPos), static_cast<float>(YPos)};
    }

    float СInput::GetCursorX()
    {
        return GetCursorPos().x;
    }

    float СInput::GetCursorY()
    {
        return GetCursorPos().y;
    }

    void СInput::SetCursorPos(FVector2 const &Pos)
    {
        СApplication     &App    = СApplication::GetInstance();
        GLFWwindow *const Window = static_cast<GLFWwindow *>(App.GetWindow().GetRawWindow());

        glfwSetCursorPos(Window, Pos.x, Pos.y);
    }
} // namespace Corvus