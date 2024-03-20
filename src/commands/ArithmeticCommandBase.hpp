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
public:
    using CommandBase::CommandBase;

protected:
    auto _PopTwoTopStackValues() const
    {
        if (_GetRuntimeContextLock()->Stack.size() < 2)
        {
            throw std::runtime_error("Not enough arguments on stack for arithmetics");
        }
        auto contextLock{ _GetRuntimeContextLock() };
        auto left = contextLock->Stack.top();
        contextLock->Stack.pop();
        auto right = contextLock->Stack.top();
        contextLock->Stack.pop();
        return std::make_tuple(left, right);
    }
};

} // namespace commands
} // namespace cpuemul
