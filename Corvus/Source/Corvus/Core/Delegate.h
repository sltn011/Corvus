#ifndef CORVUS_SOURCE_CORVUS_CORE_DELEGATE_H
#define CORVUS_SOURCE_CORVUS_CORE_DELEGATE_H

#include "Corvus/Core/Assert.h"

#include <memory>

namespace Corvus
{
    template<typename Signature>
    class TDelegate;

    template<typename Signature>
    class TMulticastDelegate;
} // namespace Corvus

#define CORVUS_DECLARE_DELEGATE(DelegateName, ReturnType, ...) \
    using DelegateName = Corvus::TDelegate<ReturnType(__VA_ARGS__)>;
#define CORVUS_DECLARE_MULTICAST_DELEGATE(DelegateName, ...) \
    using DelegateName = Corvus::TMulticastDelegate<void(__VA_ARGS__)>;

namespace Corvus
{
    template<typename R, typename... Args>
    class TDelegate<R(Args...)>
    {
    public:
        TDelegate()                             = default;
        TDelegate(TDelegate const &)            = delete;
        TDelegate(TDelegate &&)                 = default;
        TDelegate &operator=(TDelegate const &) = delete;
        TDelegate &operator=(TDelegate &&)      = default;

        bool HasBinding() const { return m_Invoker != nullptr; }

        void ClearBinding() { m_Invoker.reset(); }

        template<typename T, typename F>
        void BindObject(T *const Object, F const Method)
        {
            if (!m_Invoker)
            {
                m_Invoker = std::make_unique<TObjectInvoker<T, F, R, Args...>>(Object, Method);
            }
        }

        template<typename F>
        void BindFunction(F const Function)
        {
            if (!m_Invoker)
            {
                m_Invoker = std::make_unique<TFunctionInvoker<F, R, Args...>>(Function);
            }
        }

        void InvokeIfBound(Args &&...args) const
        {
            if (m_Invoker)
            {
                m_Invoker->Invoke(std::forward<Args>(args)...);
            }
        }

        R Invoke(Args &&...args) const
        {
            CORVUS_CORE_ASSERT_FMT(m_Invoker != nullptr, "Nothing bound to delegate!");
            return m_Invoker->Invoke(std::forward<Args>(args)...);
        }

        R operator()(Args &&...args) const { return Invoke(std::forward<Args>(args)...); }

    private:
        template<typename R, typename... Args>
        class TBaseInvoker
        {
        public:
            virtual R Invoke(Args &&...args) const = 0;
        };

        template<typename T, typename F, typename R, typename... Args>
        struct TObjectInvoker : public TBaseInvoker<R, Args...>
        {
        public:
            TObjectInvoker(T *const Object, F const Method)
            {
                m_Object = Object;
                m_Method = Method;
            }

            virtual R Invoke(Args &&...args) const override
            {
                CORVUS_CORE_ASSERT(m_Object != nullptr && m_Method != nullptr);
                return (m_Object->*m_Method)(std::forward<Args>(args)...);
            }

        private:
            T *m_Object;
            F  m_Method;
        };

        template<typename F, typename R, typename... Args>
        struct TFunctionInvoker : public TBaseInvoker<R, Args...>
        {
        public:
            TFunctionInvoker(F const Function) { m_Function = Function; }

            R Invoke(Args &&...args) const override
            {
                CORVUS_CORE_ASSERT(m_Function != nullptr);
                return (*m_Function)(std::forward<Args>(args)...);
            }

        private:
            F m_Function;
        };

        std::unique_ptr<TBaseInvoker<R, Args...>> m_Invoker;
    };

    template<typename... Args>
    class TMulticastDelegate<void(Args...)>
    {
    public:
        TMulticastDelegate()                                      = default;
        TMulticastDelegate(TMulticastDelegate const &)            = delete;
        TMulticastDelegate(TMulticastDelegate &&)                 = default;
        TMulticastDelegate &operator=(TMulticastDelegate const &) = delete;
        TMulticastDelegate &operator=(TMulticastDelegate &&)      = default;

        void ClearAllBinding() { m_Bindings.clear(); }

        template<typename T, typename F>
        void BindObject(T *const Object, F const Method)
        {
            TDelegate<void(Args...)> NewEntry;
            NewEntry.BindObject(Object, Method);
            m_Bindings.emplace_back(std::move(NewEntry));
        }

        template<typename F>
        void BindFunction(F const Function)
        {
            TDelegate<void(Args...)> NewEntry;
            NewEntry.BindFunction(Function);
            m_Bindings.emplace_back(std::move(NewEntry));
        }

        void Broadcast(Args &&...args) const
        {
            for (TDelegate<void(Args...)> const &Binding : m_Bindings)
            {
                Binding.Invoke(std::forward<Args>(args)...);
            }
        }

        void operator()(Args &&...args) const { Broadcast(std::forward<Args>(args)...); }

    private:
        std::vector<TDelegate<void(Args...)>> m_Bindings;
    };
} // namespace Corvus

#endif