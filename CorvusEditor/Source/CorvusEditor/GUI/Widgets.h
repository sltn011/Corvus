#ifndef CORVUSEDITOR_SOURCE_CORVUSEDITOR_SOURCE_GUI_WIDGETS_H
#define CORVUSEDITOR_SOURCE_CORVUSEDITOR_SOURCE_GUI_WIDGETS_H

#include "Corvus/Core/Base.h"
#include "Corvus/Math/Constants.h"
#include "Corvus/Math/Vector.h"

#include <imgui.h>

namespace Corvus::Widgets
{
    static bool DrawVecComponent(
        char const   *ComponentLabel,
        float        &Value,
        Int32         LabelColumnIndex,
        FUByteVector3 LabelBackgroundColor,
        Int32         ValueColumnIndex,
        float         DragStep,
        bool          bClamped = false,
        float         MinVal   = Corvus::Constants::Min<float>(),
        float         MaxVal   = Corvus::Constants::Max<float>()
    )
    {
        ImGui::PushID(ComponentLabel);

        ImGui::TableSetColumnIndex(LabelColumnIndex);
        ImGui::TableSetBgColor(
            ImGuiTableBgTarget_CellBg,
            IM_COL32(LabelBackgroundColor.r, LabelBackgroundColor.g, LabelBackgroundColor.b, 255),
            LabelColumnIndex
        );
        ImGui::Text(ComponentLabel);

        ImGui::TableSetColumnIndex(ValueColumnIndex);
        EEditorSliderFlags SliderFlags =
            bClamped ? EEditorSliderFlags::AlwaysClamp : EEditorSliderFlags::None;
        ImGui::SetNextItemWidth(80.0f);
        bool bUsed =
            ImGui::DragFloat("##Value", &Value, DragStep, MinVal, MaxVal, "%.3f", EnumRawValue(SliderFlags));

        ImGui::PopID();

        return bUsed;
    }

    static bool DrawVec2InputWidget(
        char const *Label,
        FVector2   &Vector,
        float       DragStep,
        bool        bClamped = false,
        FVector2    MinMax   = {Corvus::Constants::Min<float>(), Corvus::Constants::Max<float>()}
    )
    {
        bool bUsed = false;
        ImGui::Text(Label);
        if (!ImGui::BeginTable(Label, 2 * 2, EnumRawValue(EEditorTableFlags::SizingFixedFit)))
        {
            return bUsed;
        }
        ImGui::TableNextRow();
        bUsed |= DrawVecComponent("X", Vector.x, 0, {128, 0, 0}, 1, DragStep, bClamped, MinMax.x, MinMax.y);
        bUsed |= DrawVecComponent("Y", Vector.y, 2, {0, 128, 0}, 3, DragStep, bClamped, MinMax.x, MinMax.y);
        ImGui::EndTable();
        return bUsed;
    }

    static bool DrawVec3InputWidget(
        char const *Label,
        FVector3   &Vector,
        float       DragStep,
        bool        bClamped = false,
        FVector2    MinMax   = {Corvus::Constants::Min<float>(), Corvus::Constants::Max<float>()}
    )
    {
        bool bUsed = false;
        ImGui::Text(Label);
        if (!ImGui::BeginTable(Label, 3 * 2, EnumRawValue(EEditorTableFlags::SizingFixedFit)))
        {
            return bUsed;
        }
        ImGui::TableNextRow();
        bUsed |= DrawVecComponent("X", Vector.x, 0, {128, 0, 0}, 1, DragStep, bClamped, MinMax.x, MinMax.y);
        bUsed |= DrawVecComponent("Y", Vector.y, 2, {0, 128, 0}, 3, DragStep, bClamped, MinMax.x, MinMax.y);
        bUsed |= DrawVecComponent("Z", Vector.z, 4, {0, 0, 128}, 5, DragStep, bClamped, MinMax.x, MinMax.y);
        ImGui::EndTable();
        return bUsed;
    }

    static bool DrawVec4InputWidget(
        char const *Label,
        FVector4   &Vector,
        float       DragStep,
        bool        bClamped = false,
        FVector2    MinMax   = {Corvus::Constants::Min<float>(), Corvus::Constants::Max<float>()}
    )
    {
        bool bUsed = false;
        ImGui::Text(Label);
        if (!ImGui::BeginTable(Label, 4 * 2, EnumRawValue(EEditorTableFlags::SizingFixedFit)))
        {
            return bUsed;
        }
        ImGui::TableNextRow();
        bUsed |= DrawVecComponent(" X", Vector.x, 0, {128, 0, 0}, 1, DragStep, bClamped, MinMax.x, MinMax.y);
        bUsed |= DrawVecComponent("Y", Vector.y, 2, {0, 128, 0}, 3, DragStep, bClamped, MinMax.x, MinMax.y);
        bUsed |= DrawVecComponent("Z", Vector.z, 4, {0, 0, 128}, 5, DragStep, bClamped, MinMax.x, MinMax.y);
        bUsed |= DrawVecComponent("W", Vector.z, 6, {50, 50, 50}, 7, DragStep, bClamped, MinMax.x, MinMax.y);
        ImGui::EndTable();
        return bUsed;
    }

} // namespace Corvus::Widgets

#endif // !CORVUSEDITOR_SOURCE_CORVUSEDITOR_SOURCE_GUI_WIDGETS_H