#pragma once
#include "RuntimeContext.hpp"
#include "Stack/Stack.hpp"
#include <any>
#include <boost/lexical_cast.hpp>
#include <memory>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

namespace cpuemul
{
namespace commands
{

// template <class... ExpectedArgs>
// class ArgumentException : public std::runtime_error
// {
// public:
//     ArgumentException()
//         : runtime_error{
//               "Invalid arguments passed to command. Arguments recieved: " +
//               ArgumentException::_ConcatTypenames<ExpectedArgs...>() + '\n'
//           }
//     {
//     }
//
// private:
//     template <class... Args>
//     static std::string _ConcatTypenames()
//     {
//         std::string res{ "" };
//         std::vector<const char*> typeNames = { typeid(Args).name()... };
//         for (auto typeName : typeNames)
//         {
//             res += std::string{ typeName } + ", ";
//         }
//         return res;
//     }
// };

#define COMMAND_PROPERTIES(typeName, numArgs, ...)                             \
public:                                                                        \
    uint32_t GetArgumentsNumber() const override                               \
    {                                                                          \
        return numArgs;                                                        \
    }                                                                          \
    // static const typeName& Prototype()                                         \
    // {                                                                          \
    //     static typeName proto{ std::make_from_tuple<typeName>(                 \
    //         _CreateTupleWithDefaultArgs<__VA_ARGS__>()) };                     \
    //     return proto;                                                          \
    // }                                                                          \
                                                                               \
private:

namespace internal
{
template <class T, std::enable_if_t<std::is_arithmetic_v<T>, void*> = nullptr>
T ConstructFromString(const std::string& value)
{
    return boost::lexical_cast<T>(value);
}

template <class T, std::enable_if_t<std::is_constructible_v<T, std::string>,
                                    void*> = nullptr>
T ConstructFromString(const std::string& value)
{
    return T{ value };
}

template <class T, std::enable_if_t<std::is_enum_v<T>, void*> = nullptr>
T ConstructFromString(const std::string& value)
{
    return T{ value };
}

template <class T,
          std::enable_if_t<!std::is_arithmetic_v<T> &&
                               !std::is_constructible_v<T, std::string>,
                           void*> = nullptr>
T ConstructFromString(const std::string& value)
{
    throw std::runtime_error{
        "Virtual memory type is not constructible from string"
    };
}

#define TRY_RETHROW_CREATION_EXCEPTION(scope, argStr)                          \
    try                                                                        \
    {                                                                          \
        scope                                                                  \
    }                                                                          \
    catch (const boost::bad_lexical_cast& e)                                   \
    {                                                                          \
        throw std::runtime_error{                                              \
            "Failed to convert argument value \"" + argStr +                   \
                                                                               \
            "\" to virtual memory type - basic arithmetic type (" +            \
            std::string{ typeid(MemoryValueType).name() } + ")"                \
        };                                                                     \
    }                                                                          \
    catch (const std::exception& e)                                            \
    {                                                                          \
        throw std::runtime_error{                                              \
            "Exception while creating a virtual memory object (which is " +    \
            std::string{ typeid(MemoryValueType).name() } +                    \
            ") from string \"" + argStr + "\": " + std::string{ e.what() }     \
        };                                                                     \
    }                                                                          \
    catch (...)                                                                \
    {                                                                          \
        throw std::runtime_error{                                              \
            std::string{ "Unknown exception while creating a virtual memory "  \
                         "object from string \"" } +                           \
            argStr + std::string{ "\"" }                                       \
        };                                                                     \
    }

} // namespace internal

class CommandBase
{
public:
    virtual void Execute() = 0;

    static void SetRuntimeContext(std::weak_ptr<RuntimeContext>&&);

    virtual void SetArguments(const std::vector<std::string>& list) {}

    virtual uint32_t GetArgumentsNumber() const = 0;

protected:
    template <class... Args>
    static constexpr auto _CreateTupleWithDefaultArgs()
    {
        return std::tuple<Args...>{};
    }

    // TODO: to cpp
    static inline RuntimeContextPtr _GetRuntimeContextLock()
    {
        if (s_RuntimeContextPtr.expired())
        {
            throw std::runtime_error(
                "Command runtime context has not been set");
        }
        return s_RuntimeContextPtr.lock();
    }

private:
    static std::weak_ptr<RuntimeContext> s_RuntimeContextPtr;
};

} // namespace commands

} // namespace cpuemul
