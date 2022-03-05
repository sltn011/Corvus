#ifndef CORVUS_SOURCE_CORVUS_CORE_DELEGATE_H
#define CORVUS_SOURCE_CORVUS_CORE_DELEGATE_H

#include "Corvus/Core/Assert.h"

#include <memory>

namespace Corvus
{
    template<typename Signature>
    class Delegate;

    template<typename Signature>
    class MulticastDelegate;
}

#define CORVUS_DECLARE_DELEGATE(DelegateName, ReturnType, ...) using DelegateName = Corvus::Delegate<ReturnType(__VA_ARGS__)>;
#define CORVUS_DECLARE_MULTICAST_DELEGATE(DelegateName, ...) using DelegateName = Corvus::MulticastDelegate<void(__VA_ARGS__)>;

namespace Corvus
{
    template<typename R, typename... Args>
    class Delegate<R(Args...)>
    {
    public:

        Delegate() = default;
        Delegate(Delegate const &) = delete;
        Delegate(Delegate &&) = default;
        Delegate &operator=(Delegate const &) = delete;
        Delegate &operator=(Delegate &&) = default;

        bool HasBinding() const
        {
            return bool(m_Invoker);
        }

        void ClearBinding()
        {
            m_Invoker.reset();
        }

        template<typename T, typename F>
        void BindObject(T *Object, F Method)
        {
            if (!m_Invoker)
            {
                m_Invoker = std::make_unique<ObjectInvoker<T, F, R, Args...>>(Object, Method);
            }
        }

        template<typename F>
        void BindFunction(F Function)
        {
            if (!m_Invoker)
            {
                m_Invoker = std::make_unique<FunctionInvoker<F, R, Args...>>(Function);
            }
        }

        void InvokeIfBound(Args... args) const
        {
            if (m_Invoker)
            {
                m_Invoker->Invoke(std::forward<Args>(args)...);
            }
        }

        R Invoke(Args... args) const
        {
            CORVUS_CORE_ASSERT_FMT(m_Invoker != nullptr, "Nothing bound to delegate!");
            return m_Invoker->Invoke(std::forward<Args>(args)...);
        }

        R operator()(Args... args) const
        {
            return Invoke(std::forward<Args>(args)...);
        }

    protected:

        template<typename R, typename... Args>
        class BaseInvoker
        {
        public:

            virtual R Invoke(Args... args) const = 0;
        };

        template<typename T, typename F, typename R, typename... Args>
        struct ObjectInvoker : public BaseInvoker<R, Args...>
        {
        public:

            ObjectInvoker(T *Object, F Method)
            {
                m_Object = Object;
                m_Method = Method;
            }

            virtual R Invoke(Args... args) const override
            {
                CORVUS_CORE_ASSERT(m_Object != nullptr && m_Method != nullptr);
                return (m_Object->*m_Method)(std::forward<Args>(args)...);
            }

        private:

            T *m_Object;
            F  m_Method;
        };

        template<typename F, typename R, typename... Args>
        struct FunctionInvoker : public BaseInvoker<R, Args...>
        {
        public:

            FunctionInvoker(F Function)
            {
                m_Function = Function;
            }

            R Invoke(Args... args) const override
            {
                CORVUS_CORE_ASSERT(m_Function != nullptr);
                return (*m_Function)(std::forward<Args>(args)...);
            }

        private:

            F m_Function;
        };

        std::unique_ptr<BaseInvoker<R, Args...>> m_Invoker;
    };

    template<typename... Args>
    class MulticastDelegate<void(Args...)>
    {
    public:

        MulticastDelegate() = default;
        MulticastDelegate(MulticastDelegate const &) = delete;
        MulticastDelegate(MulticastDelegate &&) = default;
        MulticastDelegate &operator=(MulticastDelegate const &) = delete;
        MulticastDelegate &operator=(MulticastDelegate &&) = default;

        void ClearAllBinding()
        {
            m_Bindings.clear();
        }

        template<typename T, typename F>
        void BindObject(T *Object, F Method)
        {
            Delegate<void(Args...)> NewEntry;
            NewEntry.BindObject(Object, Method);
            m_Bindings.emplace_back(std::move(NewEntry));
        }

        template<typename F>
        void BindFunction(F Function)
        {
            Delegate<void(Args...)> NewEntry;
            NewEntry.BindFunction(Function);
            m_Bindings.emplace_back(std::move(NewEntry));
        }

        void Broadcast(Args... args) const
        {
            for (Delegate<void(Args...)> const &Binding : m_Bindings)
            {
                Binding.Invoke(std::forward<Args>(args)...);
            }
        }

        void operator()(Args... args) const
        {
            Broadcast(std::forward<Args>(args)...);
        }

    private:

        std::vector<Delegate<void(Args...)>> m_Bindings;
    };
}

#endif