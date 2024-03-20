#pragma once
#include "JmpCommandBase.hpp"

namespace cpuemul
{
namespace commands
{

class JaCommand final : public JmpCommandBase
{

public:
    static constexpr CommandCode Code = CommandCode ::Ja;
    inline CommandCode GetCommandCode() const override
    {
        return CommandCode ::Ja;
    }
    using ArgsTypeList = cul ::typelist ::TypeList<decltype(m_Label)>;
    using ArgsTupleType = std ::tuple<decltype(m_Label)>;

public:
    JaCommand()
        : JmpCommandBase(1)
    {
    }

    void Execute() override
    {
        auto [v1, v2] = _GetTwoTopStackValues();

        if (v1 > v2)
        {
            JmpCommandBase::Execute();
        }
    }
};

} // namespace commands
} // namespace cpuemul
