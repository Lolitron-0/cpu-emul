#include "FileParser.hpp"
#include "Executor.hpp"
#include "commands/Commands.hpp"
#include <memory>
#include <stdexcept>

namespace cpuemul
{

CommandBuffer FileParser::ParseSourceFile(const std::string& path)
{
    CommandBuffer result;
    result.push_back(std::make_shared<commands::PushCommand>(10));
    result.push_back(std::make_shared<commands::PushCommand>(20));
    result.push_back(std::make_shared<commands::OutCommand>());

    return result;
}

CommandBuffer FileParser::ParseBinary(const std::string& path)
{
    throw std::logic_error{ "not implemented" };
}

} // namespace cpuemul
