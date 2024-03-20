#pragma once
#include "RuntimeContext.hpp"
#include <sstream>

namespace cpuemul
{

struct ConstructionFromStringException : std::runtime_error
{
    using std::runtime_error::runtime_error;
};

struct ArgumentConstructionException : std::runtime_error
{
    using std::runtime_error::runtime_error;
};

class FileParser
{
public:
    static CommandBuffer
    ParseSourceFile(const std::string& path, std::stringstream& errStream,
                    const std::string& binaryExportPath = "");

    static CommandBuffer ParseBinary(const std::string& path,
                                     std::stringstream& errStream);

    static std::vector<std::string>
    CleanSourceFile(const std::string& path, std::stringstream& errStream);
};

} // namespace cpuemul
