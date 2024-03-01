#pragma once
#include "JmpCommandBase.hpp"

namespace cpuemul
{
namespace commands
{

class JaCommand final : public JmpCommandBase
{
    COMMAND_PROPERTIES(CommandCode::Ja, std::string);

public:
    void Execute() override
    {
        auto [v1, v2] = _GetTwoTopStackValues();

        if (v1 > v2)
        {
            JmpCommandBase::Execute();
        }
    }

private:
    std::string m_Label;
};

} // namespace commands
} // namespace cpuemul
