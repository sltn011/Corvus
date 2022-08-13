#include "CorvusPCH.h"
#include "Corvus/Memory/TestPoolable.h"

namespace Corvus
{

    POOLABLE_CLASS_IMPL(TestPoolable);

    TestPoolable::TestPoolable(int n, float f)
        : m_n{n}, m_f{f}
    {

    }

}