#pragma once
#include "Executor.hpp"
#include "RuntimeContext.hpp"
#include <iostream>

namespace cpuemul
{

class FileParser
{
public:
    static CommandBuffer ParseSourceFile(const std::string& path);

    static CommandBuffer ParseBinary(const std::string& path);

private:
};

} // namespace cpuemul
