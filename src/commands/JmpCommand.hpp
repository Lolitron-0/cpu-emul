#pragma once
#include "JmpCommandBase.hpp"

namespace cpuemul
{
namespace commands
{

class JmpCommand final : public JmpCommandBase
{
    COMMAND_PROPERTIES(CommandCode::Jmp, decltype(m_Label));

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
