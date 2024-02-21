#include "FileParser.hpp"
#include "commands/Commands.hpp"
#include <boost/lexical_cast.hpp>
#include <cul/cul.hpp>
#include <fstream>
#include <stdexcept>

namespace cpuemul
{

namespace internal
{

std::string StripNextWord(std::string& str)
{
    auto spacePos{ str.find(' ') };
    if (spacePos == std::string::npos)
    {
        auto copy{ str };
        str = "";
        return copy;
    }

    auto token{ str.substr(0, spacePos) };
    str.erase(0, spacePos + 1);
    return token;
}

template <class T>
struct CreateCommandFromCodeVisitor
{
    static void Call(commands::CommandCode code, CommandPtr& result)
    {
        if (T::GetTypeCommandCode() == code)
        {
            result = std::make_shared<T>();
        }
    }
};

} // namespace internal

CommandBuffer FileParser::ParseSourceFile(const std::string& path,
                                          std::stringstream& errStream)
{
    std::ifstream sourceFileStream{ path };
    std::vector<std::string> tokens;
    if (!sourceFileStream.is_open())
    {
        errStream << "File not found: " << path;
        return {};
    }

    CommandBuffer result;
    std::string line, token;
    int32_t lineNumber{ 0 };
    while (std::getline(sourceFileStream, line))
    {
        lineNumber++;
        if (line.empty())
        {
            continue;
        }

        token = internal::StripNextWord(line);
        auto commandCodeOpt{ commands::CommandMapping.FindIgnoreCase(token) };
        if (!commandCodeOpt.has_value())
        {
            errStream << "Unknown command on line " << lineNumber << ": \""
                      << token << "\"" << std::endl;
            break;
        }
        CommandPtr commandPtr;
        cul::typelist::ForEach<commands::CommandsTypeList,
                               internal::CreateCommandFromCodeVisitor>::
            Iterate(commandCodeOpt.value(), commandPtr);
        std::vector<std::string> args;
        for (int32_t i{ 0 }; i < commandPtr->GetArgumentsNumber(); i++)
        {
            auto argStr{ internal::StripNextWord(line) };
            if (argStr.empty())
            {
                errStream << "Not enough arguments specified for \"" << token
                          << "\" command on line " << lineNumber << std::endl;
                goto stopLineProcessing;
            }
            args.push_back(argStr);
        }
        if (!line.empty())
        {
            errStream << "Too many arguments specified for \"" + token +
                             "\" command"
                      << std::endl;
            continue;
        }
        try
        {
            commandPtr->SetArguments(args);
        }
        catch (const std::exception& e)
        {
            errStream << e.what() << std::endl;
            continue;
        }
        result.push_back(commandPtr);

    stopLineProcessing:;
    }

    sourceFileStream.close();
    return result;
}

CommandBuffer FileParser::ParseBinary(const std::string& path,
                                      std::stringstream& errStream)
{
    throw std::logic_error{ "not implemented" };
}

} // namespace cpuemul
