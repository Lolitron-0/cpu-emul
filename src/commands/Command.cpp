#include "Command.hpp"

namespace cpuemul
{
namespace commands
{

std::weak_ptr<RuntimeContext> CommandBase::s_RuntimeContextPtr{};

void CommandBase::SetRuntimeContext(std::weak_ptr<RuntimeContext>&& weakPtr)
{
    s_RuntimeContextPtr = std::move(weakPtr);
}

} // namespace commands
} // namespace cpuemul
