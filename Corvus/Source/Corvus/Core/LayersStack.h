#ifndef CORVUS_SOURCE_CORVUS_CORE_LAYERSSTACK_H
#define CORVUS_SOURCE_CORVUS_CORE_LAYERSSTACK_H

#include "Corvus/Core/Base.h"
#include "Corvus/Core/Layer.h"

#include <memory>
#include <vector>

namespace Corvus
{

    class СLayersStack
    {
    public:
        СLayersStack();
        ~СLayersStack();

        void                       PushLayer(TOwn<СLayer> NewLayer);
        [[nodiscard]] TOwn<СLayer> PopLayer();

        std::vector<TOwn<СLayer>>::iterator         Begin() { return m_Stack.begin(); }
        std::vector<TOwn<СLayer>>::iterator         End() { return m_Stack.end(); }
        std::vector<TOwn<СLayer>>::reverse_iterator RBegin() { return m_Stack.rbegin(); }
        std::vector<TOwn<СLayer>>::reverse_iterator REnd() { return m_Stack.rend(); }

        std::vector<TOwn<СLayer>>::const_iterator         Begin() const { return m_Stack.begin(); }
        std::vector<TOwn<СLayer>>::const_iterator         End() const { return m_Stack.end(); }
        std::vector<TOwn<СLayer>>::const_reverse_iterator RBegin() const { return m_Stack.rbegin(); }
        std::vector<TOwn<СLayer>>::const_reverse_iterator REnd() const { return m_Stack.rend(); }

        // For for-each loops
        std::vector<TOwn<СLayer>>::iterator begin() { return m_Stack.begin(); }
        std::vector<TOwn<СLayer>>::iterator end() { return m_Stack.end(); }

        std::vector<TOwn<СLayer>>::const_iterator begin() const { return m_Stack.begin(); }
        std::vector<TOwn<СLayer>>::const_iterator end() const { return m_Stack.end(); }

    private:
        // Front - Stack's bottom, Back - Stack's top
        std::vector<TOwn<СLayer>> m_Stack;
    };

} // namespace Corvus

#endif // !CORVUS_SOURCE_CORVUS_CORE_LAYERSSTACK_H
