#pragma once
#include <Stack/Stack.hpp>
#include <array>
#include <cul/cul.hpp>
#include <memory>
#include <unordered_map>
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

struct RegisterNameWrapper
{
    RegisterNameWrapper() = default;
    RegisterNameWrapper(const std::string& str);
    RegisterName value;
};

struct LabelTag
{
    LabelTag() = default;
    LabelTag(std::string s)
        : value{ Mapping.at(s) }
    {
    }
    size_t value;

    static std::unordered_map<std::string, size_t> Mapping;
};

using MemoryValueType = int64_t;

using CommandPtr = std::shared_ptr<commands::CommandBase>;
using CommandBuffer = std::vector<CommandPtr>;
using CommandBufferIterator = CommandBuffer::iterator;
using RegistersBuffer =
    std::array<MemoryValueType, static_cast<size_t>(RegisterName::_Count)>;
using LabelMap = std::unordered_map<int32_t, size_t>;
using MemoryStackType = stack::Stack<MemoryValueType>;
using CallStackType = stack::Stack<uint64_t>;

struct RuntimeContext
{
    bool IsRunning{ true };

    CommandBuffer Commands;
    CommandBufferIterator ExecutionIterator;

    MemoryStackType Stack;
    CallStackType CallStack;
    RegistersBuffer Registers{};
};

using RuntimeContextPtr = std::shared_ptr<RuntimeContext>;

} // namespace cpuemul
