#pragma once
#include "JmpCommandBase.hpp"

namespace cpuemul
{
namespace commands
{

class JaeCommand final : public JmpCommandBase
{

public:
    static constexpr CommandCode Code = CommandCode ::Jae;
    inline CommandCode GetCommandCode() const override
    {
        return CommandCode ::Jae;
    }
    using ArgsTypeList = cul ::typelist ::TypeList<decltype(m_Label)>;
    using ArgsTupleType = std ::tuple<decltype(m_Label)>;

public:
    JaeCommand()
        : JmpCommandBase(1)
    {
    }

    void Execute() override
    {
        auto [v1, v2] = _GetTwoTopStackValues();

        if (v1 >= v2)
        {
            JmpCommandBase::Execute();
        }
    }
};

} // namespace commands
} // namespace cpuemul
