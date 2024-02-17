#pragma once
#include "RuntimeContext.hpp"
#include <sstream>

namespace cpuemul
{

class FileParser
{
public:
    static CommandBuffer ParseSourceFile(const std::string& path,
                                         std::stringstream& errStream);

    static CommandBuffer ParseBinary(const std::string& path,
                                     std::stringstream& errStream);
};

} // namespace cpuemul
