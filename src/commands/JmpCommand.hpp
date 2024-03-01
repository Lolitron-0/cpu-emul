#pragma once
#include "JmpCommandBase.hpp"

namespace cpuemul
{
namespace commands
{

class JmpCommand final : public JmpCommandBase
{
    COMMAND_PROPERTIES(CommandCode::Jmp, std::string);

public:
    JmpCommand()
        : JmpCommandBase(1, CommandCode::Jmp)
    {
    }

    void Execute() override
    {
        JmpCommandBase::Execute();
    }

private:
    std::string m_Label;
};

} // namespace commands
} // namespace cpuemul
