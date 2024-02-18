#pragma once
#include "CommandBase.hpp"
#include "RuntimeContext.hpp"

namespace cpuemul
{
namespace commands
{

class PushCommand final : public CommandBase
{
    COMMAND_PROPERTIES(PushCommand, 1, MemoryValueType);

public:
    void Execute() override
    {
        _GetRuntimeContextLock()->stack.push(m_Value);
    }

    void SetArguments(const std::vector<std::string>& list) override
    {
        TRY_RETHROW_CREATION_EXCEPTION(
            {
                m_Value = internal::ConstructFromString<MemoryValueType>(
                    *list.begin());
            },
            *list.begin());
    }

protected:
private:
    MemoryValueType m_Value;
};

} // namespace commands
} // namespace cpuemul
