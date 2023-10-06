#ifndef CORVUS_SOURCE_CORVUS_CORE_LAYERSSTACK_H
#define CORVUS_SOURCE_CORVUS_CORE_LAYERSSTACK_H

#include "Corvus/Core/Base.h"
#include "Corvus/Core/Layer.h"

#include <memory>
#include <vector>

namespace Corvus
{

    template<typename TLayer>
    class CLayersStack
    {
    public:
        ~CLayersStack()
        {
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
        }

        void PushLayer(TOwn<TLayer> NewLayer)
        {
            if (!NewLayer)
            {
                CORVUS_CORE_ERROR("Null-layer passed into Layers stack!");
                return;
            }

            m_Stack.emplace_back(std::move(NewLayer));
            m_Stack.back()->OnPushed();
        }
        [[nodiscard]] TOwn<TLayer> PopLayer()
        {
            CORVUS_CORE_ASSERT_FMT(!m_Stack.empty(), "Can't pop layer from empty stack!");

            TOwn<TLayer> PopedLayer = std::move(m_Stack.back());
            m_Stack.erase(End() - 1);
            PopedLayer->OnPoped();

            return PopedLayer;
        }

        typename std::vector<TOwn<TLayer>>::iterator         Begin() { return m_Stack.begin(); }
        typename std::vector<TOwn<TLayer>>::iterator         End() { return m_Stack.end(); }
        typename std::vector<TOwn<TLayer>>::reverse_iterator RBegin() { return m_Stack.rbegin(); }
        typename std::vector<TOwn<TLayer>>::reverse_iterator REnd() { return m_Stack.rend(); }

        typename std::vector<TOwn<TLayer>>::const_iterator         Begin() const { return m_Stack.begin(); }
        typename std::vector<TOwn<TLayer>>::const_iterator         End() const { return m_Stack.end(); }
        typename std::vector<TOwn<TLayer>>::const_reverse_iterator RBegin() const { return m_Stack.rbegin(); }
        typename std::vector<TOwn<TLayer>>::const_reverse_iterator REnd() const { return m_Stack.rend(); }

        // For for-each loops
        typename std::vector<TOwn<TLayer>>::iterator begin() { return m_Stack.begin(); }
        typename std::vector<TOwn<TLayer>>::iterator end() { return m_Stack.end(); }

        typename std::vector<TOwn<TLayer>>::const_iterator begin() const { return m_Stack.begin(); }
        typename std::vector<TOwn<TLayer>>::const_iterator end() const { return m_Stack.end(); }

    private:
        // Front - Stack's bottom, Back - Stack's top
        std::vector<TOwn<TLayer>> m_Stack;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_CORE_LAYERSSTACK_H
