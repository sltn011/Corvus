#ifndef CORVUS_SOURCE_CORVUS_INPUT_INPUT_H
#define CORVUS_SOURCE_CORVUS_INPUT_INPUT_H

#include "Corvus/Input/KeyCodes.h"
#include "Corvus/Math/Vector.h"

namespace Corvus
{

    class CInput
    {
    public:
        // KEYBOARD

        static bool IsKeyInState(EKeyCode Key, EActionCode State);
        static bool IsKeyPressed(EKeyCode Key);
        static bool IsKeyReleased(EKeyCode Key);
        static bool IsKeyRepeated(EKeyCode Key);

        // MOUSE

        static bool IsButtonInState(EMouseCode Button, EActionCode State);
        static bool IsButtonPressed(EMouseCode Button);
        static bool IsButtonReleased(EMouseCode Button);

        // CURSOR

        static void     SetCursorEnabled(bool bEnabled);
        static FVector2 GetCursorPos();
        static float    GetCursorX();
        static float    GetCursorY();
        static void     SetCursorPos(FVector2 const &Pos);
    };

} // namespace Corvus

#endif //! CORVUS_SOURCE_CORVUS_INPUT_INPUT_H