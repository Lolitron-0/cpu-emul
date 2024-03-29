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
}

void Executor::Run()
{
    while (m_RuntimeContextPtr->ExecutionIterator !=
           m_RuntimeContextPtr->Commands.end())
    {
        try
        {
            // std::cout <<
            // (int32_t)(*m_RuntimeContextPtr->ExecutionIterator)->GetCommandCode()
            // << std::endl;
            (*m_RuntimeContextPtr->ExecutionIterator)->Execute();
        }
        catch (const std::runtime_error& e)
        {
            std::cerr << e.what() << std::endl;
            std::cerr << "Aborting" << std::endl;
            break;
        }
        if (!m_RuntimeContextPtr->IsRunning)
        {
            break;
        }
        m_RuntimeContextPtr->ExecutionIterator++;
    }
}

void Executor::LoadFromSource(const std::string& path)
{
    std::stringstream errs;
    auto commands{ FileParser::ParseSourceFile(path, errs) };
    if (errs.rdbuf()->in_avail())
    {
        std::cerr << errs.str() << std::endl;
        return;
    }
    m_RuntimeContextPtr->Commands = std::move(commands);
    _TryFindBeginCommand();
}

void Executor::LoadFromBinary(const std::string& path)
{
    std::stringstream errs;
    auto commands{ FileParser::ParseBinary(path, errs) };
    if (errs.rdbuf()->in_avail())
    {
        std::cerr << errs.str() << std::endl;
        return;
    }
    m_RuntimeContextPtr->Commands = std::move(commands);
    _TryFindBeginCommand();
}
void Executor::ExportToBinary(const std::string& sourcePath,
                              const std::string& outPath)
{
    std::stringstream errs;
    FileParser::ParseSourceFile(sourcePath, errs, outPath);
    if (errs.rdbuf()->in_avail())
    {
        std::cerr << errs.str() << std::endl;
        return;
    }
}

RuntimeContextPtr Executor::GetRuntimeContext() const
{
    return m_RuntimeContextPtr;
}

void Executor::_TryFindBeginCommand()
{
    m_RuntimeContextPtr->ExecutionIterator = std::find_if(
        m_RuntimeContextPtr->Commands.begin(),
        m_RuntimeContextPtr->Commands.end(),
        [](const auto& command)
        { return command->GetCommandCode() == commands::CommandCode::Begin; });
    if (m_RuntimeContextPtr->ExecutionIterator ==
        m_RuntimeContextPtr->Commands.end())
    {
        m_RuntimeContextPtr->ExecutionIterator =
            m_RuntimeContextPtr->Commands.begin();
    }
}

} // namespace cpuemul
