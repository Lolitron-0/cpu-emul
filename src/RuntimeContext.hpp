#pragma once
#include <Stack/Stack.hpp>
#include <array>
#include <cul/cul.hpp>
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

enum class RegisterName
{
    EAX,
    EBX,
    ECX,
    EDX,
    _Count
};

namespace internal
{
// clang-format off
constexpr cul::BiMap RegisterNameMapping{
    [](auto selector)
    {
        return selector
			.Case("eax", RegisterName::EAX)
            .Case("ebx", RegisterName::EBX)
            .Case("ecx", RegisterName::ECX)
            .Case("edx", RegisterName::EDX);
    }
};
// clang-format on
} // namespace internal

struct RegisterNameFromString
{
    RegisterNameFromString(const std::string& str)
    {
        auto opt{internal::RegisterNameMapping.FindIgnoreCase(str)};
        if (!opt.has_value())
        {
            throw std::runtime_error{ "No such register \"" + str + "\"" };
        }
        value = opt.value();
    }
    RegisterName value;
};

using CommandPtr = std::shared_ptr<commands::CommandBase>;
using CommandBuffer = std::vector<CommandPtr>;
using CommandBufferIterator = CommandBuffer::iterator;
using MemoryValueType = int64_t;
using RegistersBuffer =
    std::array<MemoryValueType, static_cast<size_t>(RegisterName::_Count)>;
using StackType = stack::Stack<MemoryValueType>;

struct RuntimeContext
{
    bool IsRunning{ true };

    CommandBuffer Commands;
    CommandBufferIterator ExecutionIterator;

    StackType Stack;
    RegistersBuffer Registers{};
    // labels
};

using RuntimeContextPtr = std::shared_ptr<RuntimeContext>;

} // namespace cpuemul
