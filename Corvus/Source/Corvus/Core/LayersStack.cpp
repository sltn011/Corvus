#include "CorvusPCH.h"

#include "Corvus/Core/LayersStack.h"

#include "Corvus/Core/Layer.h"

namespace Corvus
{

    СLayersStack::СLayersStack()
    {
    }

    СLayersStack::~СLayersStack()
    {
        // Front - Stack's bottom, Back - Stack's top
        for (auto It = RBegin(); It != REnd(); ++It)
        {
            if (*It)
            {
                (*It)->OnPoped();
            }
        }
    }

    void СLayersStack::PushLayer(TOwn<СLayer> NewLayer)
    {
        if (!NewLayer)
        {
            CORVUS_CORE_ERROR("Null-layer passed into Layers stack!");
            return;
        }

        m_Stack.emplace_back(std::move(NewLayer));
        m_Stack.back()->OnPushed();
    }

    TOwn<СLayer> СLayersStack::PopLayer()
    {
        CORVUS_CORE_ASSERT_FMT(!m_Stack.empty(), "Can't pop layer from empty stack!");

        TOwn<СLayer> PopedLayer = std::move(m_Stack.back());
        m_Stack.erase(End() - 1);
        PopedLayer->OnPoped();

        return PopedLayer;
    }

} // namespace Corvus