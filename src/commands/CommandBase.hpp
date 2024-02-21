#pragma once
#include "RuntimeContext.hpp"
#include <boost/lexical_cast.hpp>
#include <cul/cul.hpp>
#include <memory>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

namespace cpuemul
{
namespace commands
{

enum class CommandCode
{
    Begin,
    End,
    Push,
    Pop,
    Pushr,
    Popr,
    Add,
    Sub,
    Mul,
    Div,
    Out,
    In,
    Jmp,
    Jeq,
    Jne,
    Ja,
    Jae,
    Jb,
    Jbe,
    Call,
    Ret
};

// clang-format off
constexpr cul::BiMap CommandMapping{ 
	[](auto selector) 
	{
		return selector
			.Case("begin", 	commands::CommandCode::Begin)
			.Case("end", 	commands::CommandCode::End)
			.Case("push", 	commands::CommandCode::Push)
			.Case("pop", 	commands::CommandCode::Pop)
			.Case("pushr", 	commands::CommandCode::Pushr)
			.Case("popr", 	commands::CommandCode::Popr)
			.Case("add", 	commands::CommandCode::Add)
			.Case("sub", 	commands::CommandCode::Sub)
			.Case("mul", 	commands::CommandCode::Mul)
			.Case("div", 	commands::CommandCode::Div)
			.Case("out", 	commands::CommandCode::Out)
			.Case("in", 	commands::CommandCode::In);
			// .Case("jmp", 	CommandCode::Jmp)
			// .Case("jeq", 	CommandCode::Jeq)
			// .Case("jne", 	CommandCode::Jne)
			// .Case("ja", 	CommandCode::Ja)
			// .Case("jae", 	CommandCode::Jae)
			// .Case("jb", 	CommandCode::Jb)
			// .Case("jbe", 	CommandCode::Jbe)
			// .Case("call", 	CommandCode::Call)
			// .Case("ret", 	CommandCode::Ret);
	} 
};
// clang-format on

namespace internal
{

#define COMMAND_PROPERTIES(code, numArgs, ...)                                 \
public:                                                                        \
    constexpr uint32_t GetArgumentsNumber() const override                     \
    {                                                                          \
        return numArgs;                                                        \
    }                                                                          \
    constexpr CommandCode GetCommandCode() const override                      \
    {                                                                          \
        return code;                                                           \
    }                                                                          \
    constexpr static inline CommandCode GetTypeCommandCode()                   \
    {                                                                          \
        return code;                                                           \
    }

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

    virtual void SetArguments(const std::vector<std::string>& argsVec) {}

    virtual uint32_t GetArgumentsNumber() const = 0;

    virtual constexpr CommandCode GetCommandCode() const = 0;

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
