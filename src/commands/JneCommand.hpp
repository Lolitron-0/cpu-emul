#pragma once
#include "JmpCommandBase.hpp"

namespace cpuemul
{
namespace commands
{

class JneCommand final : public JmpCommandBase
{
    COMMAND_PROPERTIES(CommandCode::Jne, decltype(m_Label));

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
