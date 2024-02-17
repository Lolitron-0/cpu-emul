#pragma once
#include <Stack/Stack.hpp>
#include <memory>
#include <vector>

namespace cpuemul
{
namespace commands
{
class CommandBase;
}
} // namespace cpuemul

namespace cpuemul
{

using CommandPtr = std::shared_ptr<commands::CommandBase>;
using CommandBuffer = std::vector<CommandPtr>;
using CommandBufferIterator = CommandBuffer::iterator;
using MemoryValueType = int64_t;

struct RuntimeContext
{
    using Stack = stack::Stack<MemoryValueType>;

    CommandBuffer commands;
    CommandBufferIterator executionIterator;

    Stack stack;
    // registers
    // labels
};

using RuntimeContextPtr = std::shared_ptr<RuntimeContext>;

} // namespace cpuemul
