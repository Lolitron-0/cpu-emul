#pragma once
#include "CommandBase.hpp"
#include "RuntimeContext.hpp"

namespace cpuemul
{
namespace commands
{

class JmpCommandBase : public CommandBase
{

public:
    JmpCommandBase()
        : CommandBase{ 1, CommandCode::Jmp }
    {
    }

    void Execute() override
    {
        auto lock{ _GetRuntimeContextLock() };
        try
        {
            auto labelIdx{ lock->Labels.at(m_Label) };
            lock->ExecutionIterator =
                lock->Commands.begin() + std::max(0ul, labelIdx - 1);
        }
        catch (const std::out_of_range& e)
        {
            throw std::runtime_error{ "Unknown label: \"" + m_Label +
                                      "\" in \"" + this->GetCommandName() };
        }
    }

protected:
    void _SetArgumentsImpl(std::any argsTuple) override
    {
        auto args{ std::any_cast<std::tuple<std::string>>(argsTuple) };
        m_Label = std::get<0>(args);
    }

    auto _GetTwoTopStackValues() const
    {
        if (_GetRuntimeContextLock()->Stack.size() < 2)
        {
            throw std::runtime_error("Not enough arguments on stack for \"" +

                                     this->GetCommandName() + "\"");
        }
        auto contextLock{ _GetRuntimeContextLock() };
        auto left = contextLock->Stack.top();
        contextLock->Stack.pop();
        auto right = contextLock->Stack.top();
        contextLock->Stack.push(left);
        return std::make_tuple(left, right);
    }

private:
    std::string m_Label;
};

} // namespace commands
} // namespace cpuemul
