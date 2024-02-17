#pragma once
#include "RuntimeContext.hpp"
#include "Singleton.hpp"
#include <Stack/Stack.hpp>
#include <istream>
#include <memory>
#include <ostream>
#include <vector>

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
