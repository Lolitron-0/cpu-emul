#pragma once
#include "RuntimeContext.hpp"
#include <Stack/Stack.hpp>
#include <cul/Singleton.hpp>

namespace cpuemul
{

class Executor : public utils::SingletonMixin<Executor>
{
public:
    Executor();

    void Run();

    void LoadFromSource(const std::string& path);
    void LoadFromBinary(const std::string& path);

    RuntimeContextPtr GetRuntimeContext() const;

private:
    RuntimeContextPtr m_RuntimeContextPtr;
};

} // namespace cpuemul
