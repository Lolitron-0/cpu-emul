#pragma once
#include "RuntimeContext.hpp"
#include <any>
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
    Outr,
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
			.Case("outr", 	commands::CommandCode::Outr)
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

#define COMMAND_PROPERTIES(code, ...)                                          \
public:                                                                        \
    constexpr static inline CommandCode GetTypeCommandCode()                   \
    {                                                                          \
        return code;                                                           \
    }                                                                          \
    using ArgsTypeList = cul::typelist::TypeList<__VA_ARGS__>;                 \
    using ArgsTupleType = std::tuple<__VA_ARGS__>;                             \
                                                                               \
private:

class CommandBase
{
public:
    virtual ~CommandBase() = default;
    virtual void Execute() = 0;

    static void SetRuntimeContext(std::weak_ptr<RuntimeContext>&&);

    // virtual void SetArguments(const std::vector<std::string>& argsVec) {}

    template <class... Args>
    void SetArguments(const std::tuple<Args...>& tuple)
    {
        _SetArgumentsImpl(std::any{ tuple });
    }

    uint32_t GetArgumentsNumber() const
    {
        return m_ArgumentsNumber;
    }

    CommandCode GetCommandCode() const
    {
        return m_CommandCode;
    }

protected:
    CommandBase(uint32_t argNum, CommandCode code)
        : m_ArgumentsNumber{ argNum },
          m_CommandCode{ code }
    {
    }

    virtual void _SetArgumentsImpl(std::any) {}

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
    uint32_t m_ArgumentsNumber;
    CommandCode m_CommandCode;
};

} // namespace commands

} // namespace cpuemul
