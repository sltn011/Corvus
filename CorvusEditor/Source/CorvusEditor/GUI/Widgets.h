#ifndef CORVUSEDITOR_SOURCE_CORVUSEDITOR_SOURCE_GUI_WIDGETS_H
#define CORVUSEDITOR_SOURCE_CORVUSEDITOR_SOURCE_GUI_WIDGETS_H

#include "Corvus/Core/Base.h"
#include "Corvus/Math/Vector.h"

#include <imgui.h>

namespace Corvus::Widgets
{

    static void DrawVec2InputWidget(char const *Label, FVector2 &Vector, bool &OutSetIfChanged);
    static void DrawVec3InputWidget(char const *Label, FVector3 &Vector, bool &OutSetIfChanged);
    static void DrawVec4InputWidget(char const *Label, FVector4 &Vector, bool &OutSetIfChanged);

} // namespace Corvus::Widgets

#endif // !CORVUSEDITOR_SOURCE_CORVUSEDITOR_SOURCE_GUI_WIDGETS_H