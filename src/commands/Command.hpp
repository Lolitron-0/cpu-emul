#pragma once
#include "RuntimeContext.hpp"
#include "Stack/Stack.hpp"
#include <any>
#include <memory>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

namespace cpuemul
{
namespace commands
{

// template <class... ExpectedArgs>
// class ArgumentException : public std::runtime_error
// {
// public:
//     ArgumentException()
//         : runtime_error{
//               "Invalid arguments passed to command. Arguments recieved: " +
//               ArgumentException::_ConcatTypenames<ExpectedArgs...>() + '\n'
//           }
//     {
//     }
//
// private:
//     template <class... Args>
//     static std::string _ConcatTypenames()
//     {
//         std::string res{ "" };
//         std::vector<const char*> typeNames = { typeid(Args).name()... };
//         for (auto typeName : typeNames)
//         {
//             res += std::string{ typeName } + ", ";
//         }
//         return res;
//     }
// };

class CommandBase
{
public:
    virtual void Execute() = 0;

    static void SetRuntimeContext(std::weak_ptr<RuntimeContext>&&);

protected:
    static inline RuntimeContextPtr _GetRuntimeContextLock()
    {
        if (s_RuntimeContextPtr.expired())
        {
            throw std::runtime_error(
                "Command runtime context has not been set");
        }
		return s_RuntimeContextPtr.lock();
    }

private:
    static std::weak_ptr<RuntimeContext> s_RuntimeContextPtr;
};

} // namespace commands

} // namespace cpuemul
