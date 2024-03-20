#pragma once
#include "JmpCommandBase.hpp"

namespace cpuemul
{
namespace commands
{

class JmpCommand final : public JmpCommandBase
{

public:
    static constexpr CommandCode Code = CommandCode ::Jmp;
    inline CommandCode GetCommandCode() const override
    {
        return CommandCode ::Jmp;
    }
    using ArgsTypeList = cul ::typelist ::TypeList<decltype(m_Label)>;
    using ArgsTupleType = std ::tuple<decltype(m_Label)>;

public:
    JmpCommand()
        : JmpCommandBase(1)
    {
    }

    void Execute() override
    {
        JmpCommandBase::Execute();
    }
};

} // namespace commands
} // namespace cpuemul
