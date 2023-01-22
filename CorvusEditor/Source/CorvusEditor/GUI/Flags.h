#ifndef CORVUSEDITOR_SOURCE_CORVUSEDITOR_SOURCE_GUI_FLAGS_H
#define CORVUSEDITOR_SOURCE_CORVUSEDITOR_SOURCE_GUI_FLAGS_H

#include "Corvus/Core/Base.h"

#include <imgui.h>

namespace Corvus::GUI
{
#define CORVUSEDITOR_GENERATED_ENUM_OPS(EnumClass)                                     \
                                                                                       \
    inline constexpr std::underlying_type_t<EnumClass> RawValue(EnumClass const Flags) \
    {                                                                                  \
        using RawType = std::underlying_type_t<EnumClass>;                             \
        return static_cast<RawType>(Flags);                                            \
    }                                                                                  \
                                                                                       \
    inline constexpr EnumClass operator|(EnumClass const Lhs, EnumClass const Rhs)     \
    {                                                                                  \
        using RawType    = std::underlying_type_t<EnumClass>;                          \
        RawType RawValue = static_cast<RawType>(Lhs) | static_cast<RawType>(Rhs);      \
        return static_cast<EnumClass>(RawValue);                                       \
    }                                                                                  \
                                                                                       \
    inline constexpr EnumClass &operator|=(EnumClass &Lhs, EnumClass const Rhs)        \
    {                                                                                  \
        using RawType = std::underlying_type_t<EnumClass>;                             \
        Lhs           = Lhs | Rhs;                                                     \
        return Lhs;                                                                    \
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

    CORVUSEDITOR_GENERATED_ENUM_OPS(EDockspaceFlags);
    CORVUSEDITOR_GENERATED_ENUM_OPS(EPanelFlags);
    CORVUSEDITOR_GENERATED_ENUM_OPS(EEditorWindowFlags);

} // namespace Corvus::GUI

#endif // !CORVUSEDITOR_SOURCE_CORVUSEDITOR_SOURCE_GUI_FLAGS_H