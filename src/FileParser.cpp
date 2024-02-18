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
        return "";
    }

    auto token{ str.substr(0, spacePos) };
    str.erase(0, spacePos + 1);
    return token;
}

enum class CommandCode
{
    Begin,
    End,
    Push,
    Pop,
    Pushr,
    Popr,
    Add,
    Sub,
    Mul,
    Div,
    Out,
    In,
    Jmp,
    Jeq,
    Jne,
    Ja,
    Jae,
    Jb,
    Jbe,
    Call,
    Ret
};

// clang-format off
constexpr cul::BiMap CommandMapping{ 
	[](auto selector) {
		return selector
			// .Case("begin", 	CommandCode::Begin)
			// .Case("end", 	CommandCode::End)
			.Case("push", 	CommandCode::Push)
			.Case("pop", 	CommandCode::Pop)
			// .Case("pushr", 	CommandCode::Pushr)
			// .Case("popr", 	CommandCode::Popr)
			.Case("add", 	CommandCode::Add)
			.Case("sub", 	CommandCode::Sub)
			.Case("mul", 	CommandCode::Mul)
			.Case("div", 	CommandCode::Div)
			.Case("out", 	CommandCode::Out)
			.Case("in", 	CommandCode::In);
			// .Case("jmp", 	CommandCode::Jmp)
			// .Case("jeq", 	CommandCode::Jeq)
			// .Case("jne", 	CommandCode::Jne)
			// .Case("ja", 	CommandCode::Ja)
			// .Case("jae", 	CommandCode::Jae)
			// .Case("jb", 	CommandCode::Jb)
			// .Case("jbe", 	CommandCode::Jbe)
			// .Case("call", 	CommandCode::Call)
			// .Case("ret", 	CommandCode::Ret);
	} 
};
// clang-format on

#define MAP_COMMAND(code, type)                                                \
    case code:                                                                 \
        return std::make_shared<type>();

CommandPtr CreateCommandFromCode(CommandCode code)
{
    switch (code)
    {
        MAP_COMMAND(CommandCode::Push, commands::PushCommand)
        MAP_COMMAND(CommandCode::Pop, commands::PopCommand)
        MAP_COMMAND(CommandCode::Add, commands::AddCommand)
        MAP_COMMAND(CommandCode::Sub, commands::SubCommand)
        MAP_COMMAND(CommandCode::Mul, commands::MulCommand)
        MAP_COMMAND(CommandCode::Div, commands::DivCommand)
        MAP_COMMAND(CommandCode::In, commands::InCommand)
        MAP_COMMAND(CommandCode::Out, commands::OutCommand)
    default:
        throw std::logic_error{ "Unknown command code: " +
                                std::string{ CommandMapping.Find(code).value_or(
                                    "UNMAPPED") } };
    }
}

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

    std::string line, token;
    int32_t lineNumber{ 1 };
    while (std::getline(sourceFileStream, line))
    {
        while (!line.empty())
        {
            token = internal::StripNextWord(line);
            auto commandCodeOpt{ internal::CommandMapping.FindIgnoreCase(
                token) };
            if (!commandCodeOpt.has_value())
            {
                errStream << "Unknown command on line " << lineNumber << '\n';
                break;
            }
            auto commandPtr{ internal::CreateCommandFromCode(
                commandCodeOpt.value()) };
            std::vector<std::string> args;
            for (int32_t i{ 0 }; i < commandPtr->GetArgumentsNumber(); i++)
            {
                auto argStr{ internal::StripNextWord(line) };
                if (argStr.empty())
                {
                    errStream << "Not enough arguments specified for \""
                              << token << "\" command on line " << lineNumber
                              << std::endl;
                    goto stopLineProcessing;
                }
                args.push_back(argStr);
            }
        }
    stopLineProcessing:;
        lineNumber++;
    }

    sourceFileStream.close();
}

CommandBuffer FileParser::ParseBinary(const std::string& path,
                                      std::stringstream& errStream)
{
    throw std::logic_error{ "not implemented" };
}

} // namespace cpuemul
