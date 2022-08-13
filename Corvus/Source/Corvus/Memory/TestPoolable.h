#ifndef TESTPOOLABLE
#define TESTPOOLABLE

#include "Corvus/Memory/PoolRegistry.h"

namespace Corvus
{

    class TestPoolable
    {

        POOLABLE_CLASS_BODY();

    public:

        TestPoolable(int n, float f);

        int m_n;
        float m_f;

    };

}

#endif