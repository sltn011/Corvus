#include "CorvusPCH.h"

#include "Corvus/Core/Application.h"
#include "Corvus/Input/Input.h"

#include <GLFW/glfw3.h>

namespace Corvus
{
    bool CInput::IsKeyInState(EKeyCode const Key, EActionCode const State)
    {
        CApplication     &App    = CApplication::GetInstance();
        GLFWwindow *const Window = static_cast<GLFWwindow *>(App.GetWindow().GetRawWindow());
        return glfwGetKey(Window, static_cast<int>(Key)) == State;
    }

    bool CInput::IsKeyPressed(EKeyCode const Key)
    {
        return IsKeyInState(Key, Action::Press);
    }

    bool CInput::IsKeyReleased(EKeyCode const Key)
    {
        return IsKeyInState(Key, Action::Release);
    }

    bool CInput::IsKeyRepeated(EKeyCode const Key)
    {
        return IsKeyInState(Key, Action::Repeat);
    }

    bool CInput::IsButtonInState(EMouseCode const Button, EActionCode const State)
    {
        CApplication     &App    = CApplication::GetInstance();
        GLFWwindow *const Window = static_cast<GLFWwindow *>(App.GetWindow().GetRawWindow());
        return glfwGetMouseButton(Window, static_cast<int>(Button)) == State;
    }

    bool CInput::IsButtonPressed(EMouseCode const Button)
    {
        return IsButtonInState(Button, Action::Press);
    }

    bool CInput::IsButtonReleased(EMouseCode const Button)
    {
        return IsButtonInState(Button, Action::Release);
    }

    void CInput::SetCursorEnabled(bool const bEnabled)
    {
        CApplication     &App    = CApplication::GetInstance();
        GLFWwindow *const Window = static_cast<GLFWwindow *>(App.GetWindow().GetRawWindow());

        glfwSetInputMode(Window, GLFW_CURSOR, bEnabled ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
    }

    FVector2 CInput::GetCursorPos()
    {
        CApplication     &App    = CApplication::GetInstance();
        GLFWwindow *const Window = static_cast<GLFWwindow *>(App.GetWindow().GetRawWindow());

        double XPos, YPos;
        glfwGetCursorPos(Window, &XPos, &YPos);

        return FVector2{static_cast<float>(XPos), static_cast<float>(YPos)};
    }

    float CInput::GetCursorX()
    {
        return GetCursorPos().x;
    }

    float CInput::GetCursorY()
    {
        return GetCursorPos().y;
    }

    void CInput::SetCursorPos(FVector2 const &Pos)
    {
        CApplication     &App    = CApplication::GetInstance();
        GLFWwindow *const Window = static_cast<GLFWwindow *>(App.GetWindow().GetRawWindow());

        glfwSetCursorPos(Window, Pos.x, Pos.y);
    }
} // namespace Corvus