#pragma once
#include "JmpCommandBase.hpp"

namespace cpuemul
{
namespace commands
{

class JbCommand final : public JmpCommandBase
{
    COMMAND_PROPERTIES(CommandCode::Jb, std::string);

public:
    JbCommand()
        : JmpCommandBase(1, CommandCode::Jb)
    {
    }

    void Execute() override
    {
        auto [v1, v2] = _GetTwoTopStackValues();

        if (v1 < v2)
        {
            JmpCommandBase::Execute();
        }
    }

private:
    std::string m_Label;
};

} // namespace commands
} // namespace cpuemul
