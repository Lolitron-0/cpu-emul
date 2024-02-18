#include "Executor.hpp"
#include "FileParser.hpp"
#include "commands/Commands.hpp"
#include <memory>

namespace cpuemul
{

Executor::Executor()
{
    m_RuntimeContextPtr = std::make_shared<RuntimeContext>();
    commands::Command::SetRuntimeContext(m_RuntimeContextPtr);

    // TODO: find begin command
}

void Executor::Run()
{
    for (auto commandPtr : m_RuntimeContextPtr->commands)
    {
        commandPtr->Execute();
    }
}

void Executor::LoadFromSource(const std::string& path)
{
    std::stringstream errs;
    auto res = FileParser::ParseSourceFile(path, errs);
    if (errs.rdbuf()->in_avail())
    {
        std::cerr << errs.str() << std::endl;
    }
    m_RuntimeContextPtr->commands = std::move(res);
    m_RuntimeContextPtr->executionIterator =
        m_RuntimeContextPtr->commands.begin();
}

void Executor::LoadFromBinary(const std::string& path)
{
    std::stringstream errs;
    m_RuntimeContextPtr->commands = FileParser::ParseBinary(path, errs);
    m_RuntimeContextPtr->executionIterator =
        m_RuntimeContextPtr->commands.begin();
}

RuntimeContextPtr Executor::GetRuntimeContext() const
{
    return m_RuntimeContextPtr;
}

} // namespace cpuemul
