#ifndef CORVUS_SOURCE_CORVUS_CORE_INPUT_H
#define CORVUS_SOURCE_CORVUS_CORE_INPUT_H

#include "Corvus/Core/KeyCodes.h"
#include "Corvus/Math/Vector.h"

namespace Corvus
{

    struct Input
    {
        // KEYBOARD

        static bool IsKeyInState(KeyCode Key, ActionCode State);
        static bool IsKeyPressed(KeyCode Key);
        static bool IsKeyReleased(KeyCode Key);
        static bool IsKeyRepeated(KeyCode Key);

        // MOUSE

        static bool IsButtonInState(MouseCode Button, ActionCode State);
        static bool IsButtonPressed(MouseCode Button);
        static bool IsButtonReleased(MouseCode Button);

        // CURSOR

        static void  SetCursorEnabled(bool bEnabled);
        static Vec2  GetCursorPos();
        static float GetCursorX();
        static float GetCursorY();
        static void  SetCursorPos(Vec2 Pos);
    };

} // namespace Corvus

#endif //! CORVUS_SOURCE_CORVUS_CORE_INPUT_H