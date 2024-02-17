#include "FileParser.hpp"
#include "commands/Commands.hpp"
#include <cul/cul.hpp>
#include <fstream>
#include <stdexcept>

namespace cpuemul
{

namespace
{
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
constexpr cul::BiMap commandMapping{ 
	[](auto selector) {
		return selector
			.Case("begin", 	CommandCode::Begin)
			.Case("end", 	CommandCode::End)
			.Case("push", 	CommandCode::Push)
			.Case("pop", 	CommandCode::Pop)
			.Case("pushr", 	CommandCode::Pushr)
			.Case("popr", 	CommandCode::Popr)
			.Case("add", 	CommandCode::Add)
			.Case("sub", 	CommandCode::Sub)
			.Case("mul", 	CommandCode::Mul)
			.Case("div", 	CommandCode::Div)
			.Case("out", 	CommandCode::Out)
			.Case("in", 	CommandCode::In)
			.Case("jmp", 	CommandCode::Jmp)
			.Case("jeq", 	CommandCode::Jeq)
			.Case("jne", 	CommandCode::Jne)
			.Case("ja", 	CommandCode::Ja)
			.Case("jae", 	CommandCode::Jae)
			.Case("jb", 	CommandCode::Jb)
			.Case("jbe", 	CommandCode::Jbe)
			.Case("call", 	CommandCode::Call)
			.Case("ret", 	CommandCode::Ret);
	} 
};
// clang-format on
} // namespace

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

    std::string line;
    int32_t lineNumber{ 1 };
    while (std::getline(sourceFileStream, line))
    {
		
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
