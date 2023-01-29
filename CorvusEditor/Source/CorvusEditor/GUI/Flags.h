#ifndef CORVUSEDITOR_SOURCE_CORVUSEDITOR_SOURCE_GUI_FLAGS_H
#define CORVUSEDITOR_SOURCE_CORVUSEDITOR_SOURCE_GUI_FLAGS_H

#include "Corvus/Core/Base.h"

#include <imgui.h>

namespace Corvus
{
#define CORVUSEDITOR_GENERATED_ENUM_OPS(EnumClass)                                 \
                                                                                   \
    inline constexpr EnumClass operator|(EnumClass const Lhs, EnumClass const Rhs) \
    {                                                                              \
        using RawType    = std::underlying_type_t<EnumClass>;                      \
        RawType RawValue = static_cast<RawType>(Lhs) | static_cast<RawType>(Rhs);  \
        return static_cast<EnumClass>(RawValue);                                   \
    }                                                                              \
                                                                                   \
    inline constexpr EnumClass &operator|=(EnumClass &Lhs, EnumClass const Rhs)    \
    {                                                                              \
        using RawType = std::underlying_type_t<EnumClass>;                         \
        Lhs           = Lhs | Rhs;                                                 \
        return Lhs;                                                                \
    }

    enum class EDockspaceFlags : UInt32
    {
        None          = ImGuiDockNodeFlags_None,
        KeepAliveOnly = ImGuiDockNodeFlags_KeepAliveOnly,
        // NoCentralNode          = ImGuiDockNodeFlags_NoCentralNode,
        NoDockingInCentralNode = ImGuiDockNodeFlags_NoDockingInCentralNode,
        PassthruCentralNode    = ImGuiDockNodeFlags_PassthruCentralNode,
        NoSplit                = ImGuiDockNodeFlags_NoSplit,
        NoResize               = ImGuiDockNodeFlags_NoResize,
        AutoHideTabBar         = ImGuiDockNodeFlags_AutoHideTabBar,
    };

    enum class EPanelFlags : UInt32
    {
        None                      = ImGuiWindowFlags_None,
        NoTitleBar                = ImGuiWindowFlags_NoTitleBar,
        NoResize                  = ImGuiWindowFlags_NoResize,
        NoMove                    = ImGuiWindowFlags_NoMove,
        NoScrollbar               = ImGuiWindowFlags_NoScrollbar,
        NoScrollWithMouse         = ImGuiWindowFlags_NoScrollWithMouse,
        NoCollapse                = ImGuiWindowFlags_NoCollapse,
        AlwaysAutoResize          = ImGuiWindowFlags_AlwaysAutoResize,
        NoBackground              = ImGuiWindowFlags_NoBackground,
        NoSavedSettings           = ImGuiWindowFlags_NoSavedSettings,
        NoMouseInputs             = ImGuiWindowFlags_NoMouseInputs,
        MenuBar                   = ImGuiWindowFlags_MenuBar,
        HorizontalScrollbar       = ImGuiWindowFlags_HorizontalScrollbar,
        NoFocusOnAppearing        = ImGuiWindowFlags_NoFocusOnAppearing,
        NoBringToFrontOnFocus     = ImGuiWindowFlags_NoBringToFrontOnFocus,
        AlwaysVerticalScrollbar   = ImGuiWindowFlags_AlwaysVerticalScrollbar,
        AlwaysHorizontalScrollbar = ImGuiWindowFlags_AlwaysHorizontalScrollbar,
        AlwaysUseWindowPadding    = ImGuiWindowFlags_AlwaysUseWindowPadding,
        NoNavInputs               = ImGuiWindowFlags_NoNavInputs,
        NoNavFocus                = ImGuiWindowFlags_NoNavFocus,
        UnsavedDocument           = ImGuiWindowFlags_UnsavedDocument,
        NoDocking                 = ImGuiWindowFlags_NoDocking,

        NoNav        = NoNavInputs | NoNavFocus,
        NoDecoration = NoTitleBar | NoResize | NoScrollbar | NoCollapse,
        NoInputs     = NoMouseInputs | NoNavInputs | NoNavFocus,
    };

    enum class EEditorWindowFlags : UInt32
    {
        None                      = ImGuiWindowFlags_None,
        NoTitleBar                = ImGuiWindowFlags_NoTitleBar,
        NoResize                  = ImGuiWindowFlags_NoResize,
        NoMove                    = ImGuiWindowFlags_NoMove,
        NoScrollbar               = ImGuiWindowFlags_NoScrollbar,
        NoScrollWithMouse         = ImGuiWindowFlags_NoScrollWithMouse,
        NoCollapse                = ImGuiWindowFlags_NoCollapse,
        AlwaysAutoResize          = ImGuiWindowFlags_AlwaysAutoResize,
        NoBackground              = ImGuiWindowFlags_NoBackground,
        NoSavedSettings           = ImGuiWindowFlags_NoSavedSettings,
        NoMouseInputs             = ImGuiWindowFlags_NoMouseInputs,
        MenuBar                   = ImGuiWindowFlags_MenuBar,
        HorizontalScrollbar       = ImGuiWindowFlags_HorizontalScrollbar,
        NoFocusOnAppearing        = ImGuiWindowFlags_NoFocusOnAppearing,
        NoBringToFrontOnFocus     = ImGuiWindowFlags_NoBringToFrontOnFocus,
        AlwaysVerticalScrollbar   = ImGuiWindowFlags_AlwaysVerticalScrollbar,
        AlwaysHorizontalScrollbar = ImGuiWindowFlags_AlwaysHorizontalScrollbar,
        AlwaysUseWindowPadding    = ImGuiWindowFlags_AlwaysUseWindowPadding,
        NoNavInputs               = ImGuiWindowFlags_NoNavInputs,
        NoNavFocus                = ImGuiWindowFlags_NoNavFocus,
        UnsavedDocument           = ImGuiWindowFlags_UnsavedDocument,
        NoDocking                 = ImGuiWindowFlags_NoDocking,

        NoNav        = NoNavInputs | NoNavFocus,
        NoDecoration = NoTitleBar | NoResize | NoScrollbar | NoCollapse,
        NoInputs     = NoMouseInputs | NoNavInputs | NoNavFocus,
    };

    enum class EEditorSliderFlags : UInt32
    {
        None            = ImGuiSliderFlags_None,
        AlwaysClamp     = ImGuiSliderFlags_AlwaysClamp,
        Logarithmic     = ImGuiSliderFlags_Logarithmic,
        NoRoundToFormat = ImGuiSliderFlags_NoRoundToFormat,
        NoInput         = ImGuiSliderFlags_NoInput,
    };

    enum class EEditorTableFlags : UInt32
    {
        None              = ImGuiTableFlags_None,
        Resizable         = ImGuiTableFlags_Resizable,
        Reorderable       = ImGuiTableFlags_Reorderable,
        Hideable          = ImGuiTableFlags_Hideable,
        Sortable          = ImGuiTableFlags_Sortable,
        NoSavedSettings   = ImGuiTableFlags_NoSavedSettings,
        ContextMenuInBody = ImGuiTableFlags_ContextMenuInBody,
        // Decorations
        RowBg                      = ImGuiTableFlags_RowBg,
        BordersInnerH              = ImGuiTableFlags_BordersInnerH,
        BordersOuterH              = ImGuiTableFlags_BordersOuterH,
        BordersInnerV              = ImGuiTableFlags_BordersInnerV,
        BordersOuterV              = ImGuiTableFlags_BordersOuterV,
        BordersH                   = BordersInnerH | BordersOuterH,
        BordersV                   = BordersInnerV | BordersOuterV,
        BordersInner               = BordersInnerV | BordersInnerH,
        BordersOuter               = BordersOuterV | BordersOuterH,
        Borders                    = BordersInner | BordersOuter,
        NoBordersInBody            = ImGuiTableFlags_NoBordersInBody,
        NoBordersInBodyUntilResize = ImGuiTableFlags_NoBordersInBodyUntilResize,
        // Sizing Policy
        SizingFixedFit    = ImGuiTableFlags_SizingFixedFit,
        SizingFixedSame   = ImGuiTableFlags_SizingFixedSame,
        SizingStretchProp = ImGuiTableFlags_SizingStretchProp,
        SizingStretchSame = ImGuiTableFlags_SizingStretchSame,
        // Sizing Extra Options
        NoHostExtendX        = ImGuiTableFlags_NoHostExtendX,
        NoHostExtendY        = ImGuiTableFlags_NoHostExtendY,
        NoKeepColumnsVisible = ImGuiTableFlags_NoKeepColumnsVisible,
        PreciseWidths        = ImGuiTableFlags_PreciseWidths,
        // Clipping
        NoClip = ImGuiTableFlags_NoClip,
        // Padding
        PadOuterX   = ImGuiTableFlags_PadOuterX,
        NoPadOuterX = ImGuiTableFlags_NoPadOuterX,
        NoPadInnerX = ImGuiTableFlags_NoPadInnerX,
        // Scrolling
        ScrollX = ImGuiTableFlags_ScrollX,
        ScrollY = ImGuiTableFlags_ScrollY,
        // Sorting
        SortMulti    = ImGuiTableFlags_SortMulti,
        SortTristate = ImGuiTableFlags_SortTristate,
    };

    enum class EEditorTableColumnFlags : UInt32
    {
        None                 = ImGuiTableColumnFlags_None,
        Disabled             = ImGuiTableColumnFlags_Disabled,
        DefaultHide          = ImGuiTableColumnFlags_DefaultHide,
        DefaultSort          = ImGuiTableColumnFlags_DefaultSort,
        WidthStretch         = ImGuiTableColumnFlags_WidthStretch,
        WidthFixed           = ImGuiTableColumnFlags_WidthFixed,
        NoResize             = ImGuiTableColumnFlags_NoResize,
        NoReorder            = ImGuiTableColumnFlags_NoReorder,
        NoHide               = ImGuiTableColumnFlags_NoHide,
        NoClip               = ImGuiTableColumnFlags_NoClip,
        NoSort               = ImGuiTableColumnFlags_NoSort,
        NoSortAscending      = ImGuiTableColumnFlags_NoSortAscending,
        NoSortDescending     = ImGuiTableColumnFlags_NoSortDescending,
        NoHeaderLabel        = ImGuiTableColumnFlags_NoHeaderLabel,
        NoHeaderWidth        = ImGuiTableColumnFlags_NoHeaderWidth,
        PreferSortAscending  = ImGuiTableColumnFlags_PreferSortAscending,
        PreferSortDescending = ImGuiTableColumnFlags_PreferSortDescending,
        IndentEnable         = ImGuiTableColumnFlags_IndentEnable,
        IndentDisable        = ImGuiTableColumnFlags_IndentDisable,
    };

    CORVUSEDITOR_GENERATED_ENUM_OPS(EDockspaceFlags);
    CORVUSEDITOR_GENERATED_ENUM_OPS(EPanelFlags);
    CORVUSEDITOR_GENERATED_ENUM_OPS(EEditorWindowFlags);
    CORVUSEDITOR_GENERATED_ENUM_OPS(EEditorSliderFlags);
    CORVUSEDITOR_GENERATED_ENUM_OPS(EEditorTableFlags);
    CORVUSEDITOR_GENERATED_ENUM_OPS(EEditorTableColumnFlags);

} // namespace Corvus

#endif // !CORVUSEDITOR_SOURCE_CORVUSEDITOR_SOURCE_GUI_FLAGS_H