#include "CorvusPCH.h"

#include "Corvus/Core/LayersStack.h"

#include "Corvus/Core/Layer.h"

namespace Corvus
{

    CLayersStack::CLayersStack()
    {
    }

    CLayersStack::~CLayersStack()
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

    void CLayersStack::PushLayer(TOwn<CLayer> NewLayer)
    {
        if (!NewLayer)
        {
            CORVUS_CORE_ERROR("Null-layer passed into Layers stack!");
            return;
        }

        m_Stack.emplace_back(std::move(NewLayer));
        m_Stack.back()->OnPushed();
    }

    void CLayersStack::PopLayer()
    {
        CORVUS_CORE_ASSERT_FMT(!m_Stack.empty(), "Can't pop layer from empty stack!");

        TOwn<CLayer> PopedLayer = std::move(m_Stack.back());
        m_Stack.erase(End() - 1);
        PopedLayer->OnPoped();
        PopedLayer.reset();
    }

    TOwn<CLayer> &CLayersStack::TopLayer()
    {
        CORVUS_CORE_ASSERT_FMT(!Empty(), "Can't get top layer from empty stack!");
        return m_Stack.back();
    }

} // namespace Corvus