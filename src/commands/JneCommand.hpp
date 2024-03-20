#pragma once
#include "JmpCommandBase.hpp"

namespace cpuemul
{
namespace commands
{

class JneCommand final : public JmpCommandBase
{

public:
    static constexpr CommandCode Code = CommandCode ::Jne;
    inline CommandCode GetCommandCode() const override
    {
        return CommandCode ::Jne;
    }
    using ArgsTypeList = cul ::typelist ::TypeList<decltype(m_Label)>;
    using ArgsTupleType = std ::tuple<decltype(m_Label)>;

public:
    JneCommand()
        : JmpCommandBase(1)
    {
    }

    void Execute() override
    {
        auto [v1, v2] = _GetTwoTopStackValues();

        if (v1 != v2)
        {
            JmpCommandBase::Execute();
        }
    }

private:
};

} // namespace commands
} // namespace cpuemul
