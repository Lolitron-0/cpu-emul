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
    void Execute() override
    {
        JmpCommandBase::Execute();
    }

private:
    std::string m_Label;
};

} // namespace commands
} // namespace cpuemul
