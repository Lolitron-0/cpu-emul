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
        if (_GetRuntimeContextLock()->Stack.size() < 2)
        {
            throw std::runtime_error(
                "Not enough arguments on stack for \"" +
                std::string{
                    CommandMapping.Find(this->GetCommandCode()).value() } +
                "\"");
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
