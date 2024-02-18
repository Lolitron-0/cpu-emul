#pragma once
#include "CommandBase.hpp"
#include "RuntimeContext.hpp"
#include <tuple>

namespace cpuemul
{
namespace commands
{

class ArithmeticCommandBase : public CommandBase
{

protected:
    auto _GetTwoTopStackValues() const
    {
        auto contextLock{ _GetRuntimeContextLock() };
        auto left = contextLock->stack.top();
        contextLock->stack.pop();
        auto right = contextLock->stack.top();
        contextLock->stack.pop();
        return std::make_tuple(left, right);
    }
};

} // namespace commands
} // namespace cpuemul
