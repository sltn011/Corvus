#ifndef CORVUS_SOURCE_CORVUS_CORE_INPUT_H
#define CORVUS_SOURCE_CORVUS_CORE_INPUT_H

#include "Corvus/Core/KeyCodes.h"

#include <glm/glm.hpp>

namespace Corvus
{

    struct Input
    {
        // Check if keyboard key is in specified state
        static bool IsKeyInState(KeyCode Key, ActionCode State);

        // Check if keyboard key is pressed
        static bool IsKeyPressed(KeyCode Key);

        // Check if keyboard key is released
        static bool IsKeyReleased(KeyCode Key);

        // Check if keyboard key is repeated
        static bool IsKeyRepeated(KeyCode Key);



        // Check if mouse button is in specified state
        static bool IsButtonInState(MouseCode Button, ActionCode State);

        // Check if mouse button is pressed
        static bool IsButtonPressed(MouseCode Button);

        // Check if mouse button is released
        static bool IsButtonReleased(MouseCode Button);



        static glm::vec2 GetCursorPos();
        static float GetCursorX();
        static float GetCursorY();
    };

}

#endif //!CORVUS_SOURCE_CORVUS_CORE_INPUT_H