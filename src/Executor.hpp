#pragma once
#include "RuntimeContext.hpp"
#include <Stack/Stack.hpp>
#include <cul/cul.hpp>

namespace cpuemul
{

class Executor : public cul::SingletonMixin<Executor>
{
public:
    Executor();

    void Run();

    void LoadFromSource(const std::string& path);
    void LoadFromBinary(const std::string& path);
    void ExportToBinary(const std::string& sourcePath, const std::string& outPath);

    RuntimeContextPtr GetRuntimeContext() const;

private:
	void _TryFindBeginCommand();

private:
    RuntimeContextPtr m_RuntimeContextPtr;
};

} // namespace cpuemul
