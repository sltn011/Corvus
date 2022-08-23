#ifndef CORVUS_SOURCE_CORVUS_CORE_LAYERSSTACK_H
#define CORVUS_SOURCE_CORVUS_CORE_LAYERSSTACK_H

#include "Corvus/Core/Base.h"
#include "Corvus/Core/Layer.h"

#include <memory>
#include <vector>

namespace Corvus
{

    class LayersStack
    {
    public:
        LayersStack();
        ~LayersStack();

        void                      PushLayer(TOwn<Layer> NewLayer);
        [[nodiscard]] TOwn<Layer> PopLayer();

        std::vector<TOwn<Layer>>::iterator         Begin() { return m_Stack.begin(); }
        std::vector<TOwn<Layer>>::iterator         End() { return m_Stack.end(); }
        std::vector<TOwn<Layer>>::reverse_iterator RBegin() { return m_Stack.rbegin(); }
        std::vector<TOwn<Layer>>::reverse_iterator REnd() { return m_Stack.rend(); }

        std::vector<TOwn<Layer>>::const_iterator         Begin() const { return m_Stack.begin(); }
        std::vector<TOwn<Layer>>::const_iterator         End() const { return m_Stack.end(); }
        std::vector<TOwn<Layer>>::const_reverse_iterator RBegin() const { return m_Stack.rbegin(); }
        std::vector<TOwn<Layer>>::const_reverse_iterator REnd() const { return m_Stack.rend(); }

        // For for-each loops
        std::vector<TOwn<Layer>>::iterator begin() { return m_Stack.begin(); }
        std::vector<TOwn<Layer>>::iterator end() { return m_Stack.end(); }

        std::vector<TOwn<Layer>>::const_iterator begin() const { return m_Stack.begin(); }
        std::vector<TOwn<Layer>>::const_iterator end() const { return m_Stack.end(); }

    private:
        // Front - Stack's bottom, Back - Stack's top
        std::vector<TOwn<Layer>> m_Stack;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_CORE_LAYERSSTACK_H
