#pragma once
#include "CommandBase.hpp"
#include "RuntimeContext.hpp"

namespace cpuemul
{
namespace commands
{

class PushCommand final : public CommandBase
{
    COMMAND_PROPERTIES(CommandCode::Push, 1, MemoryValueType);

public:
    void Execute() override
    {
        _GetRuntimeContextLock()->stack.push(m_Value);
    }

    void SetArguments(const std::vector<std::string>& argsVec) override
    {
        TRY_RETHROW_CREATION_EXCEPTION(
            {
                m_Value = internal::ConstructFromString<MemoryValueType>(
                    *argsVec.begin());
            },
            *argsVec.begin());
    }

protected:
private:
    MemoryValueType m_Value;
};

} // namespace commands
} // namespace cpuemul
