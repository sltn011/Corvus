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

        void PushLayer(Own<Layer> NewLayer);
        [[nodiscard]] Own<Layer> PopLayer();

        std::vector<Own<Layer>>::iterator Begin() { return m_Stack.begin(); }
        std::vector<Own<Layer>>::iterator End() { return m_Stack.end(); }
        std::vector<Own<Layer>>::reverse_iterator RBegin() { return m_Stack.rbegin(); }
        std::vector<Own<Layer>>::reverse_iterator REnd() { return m_Stack.rend(); }

        std::vector<Own<Layer>>::const_iterator Begin() const { return m_Stack.begin(); }
        std::vector<Own<Layer>>::const_iterator End() const { return m_Stack.end(); }
        std::vector<Own<Layer>>::const_reverse_iterator RBegin() const { return m_Stack.rbegin(); }
        std::vector<Own<Layer>>::const_reverse_iterator REnd() const { return m_Stack.rend(); }


        // For for-each loops
        std::vector<Own<Layer>>::iterator begin() { return m_Stack.begin(); }
        std::vector<Own<Layer>>::iterator end() { return m_Stack.end(); }

        std::vector<Own<Layer>>::const_iterator begin() const { return m_Stack.begin(); }
        std::vector<Own<Layer>>::const_iterator end() const { return m_Stack.end(); }

    protected:

        // Front - Stack's bottom, Back - Stack's top
        std::vector<Own<Layer>> m_Stack;
    };

}

#endif // !CORVUS_SOURCE_CORVUS_CORE_LAYERSSTACK_H
