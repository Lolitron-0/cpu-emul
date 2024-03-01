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
    static std::tuple<CommandBuffer, LabelMap>
    ParseSourceFile(const std::string& path, std::stringstream& errStream);

    static CommandBuffer ParseBinary(const std::string& path,
                                     std::stringstream& errStream);
};

} // namespace cpuemul
