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
        auto copy{ str };
        str = "";
        return copy;
    }

    auto token{ str.substr(0, spacePos) };
    str.erase(0, spacePos + 1);
    return token;
}

template <class T>
struct CreateCommandFromCodeVisitor
{
    static void Call(commands::CommandCode code, CommandPtr& result)
    {
        if (T::GetTypeCommandCode() == code)
        {
            result = std::make_shared<T>();
        }
    }
};

// template <class A, template <class...> class B>
// struct Rebind;
//
// template <template <class...> class A, class... T, template <class...> class
// B> struct Rebind<A<T...>, B>
// {
//     using type = B<T...>;
// };
//
// template <class A, template <class...> class B>
// using Rebind_t = Rebind<A, B>::type;

//////////////////////////

template <class Tuple, class F,
          class Indices = std::make_index_sequence<std::tuple_size_v<Tuple>>>
struct RuntimeGetFunctionTable;

template <class Tuple, class F, size_t I>
decltype(auto) ApplyForIndex(Tuple& t, F f)
{
    f(std::get<I>(t));
}

template <class Tuple, class F, size_t... Indices>
struct RuntimeGetFunctionTable<Tuple, F, std::index_sequence<Indices...>>
{
    using FuncType = void (*)(Tuple&, F);
    static constexpr FuncType table[]{
        { &ApplyForIndex<Tuple, F, Indices>... }
    };
};

template <class Tuple, class F>
void RuntimeGet(Tuple& t, size_t index, F f)
{
    using TupleType = std::remove_reference_t<Tuple>;
    if (index >= std::tuple_size_v<TupleType>)
    {
        throw std::runtime_error{ "Tuple get out of range" };
    }
    RuntimeGetFunctionTable<Tuple, F>::table[index](t, f);
}

//////////////////////////

template <class T, std::enable_if_t<std::is_arithmetic_v<T>, void*> = nullptr>
T ConstructFromString(const std::string& value)
{
    return boost::lexical_cast<T>(value);
}

template <class T, std::enable_if_t<std::is_constructible_v<T, std::string>,
                                    void*> = nullptr>
T ConstructFromString(const std::string& value)
{
    return T{ value };
}

template <class T,
          std::enable_if_t<!std::is_arithmetic_v<T> &&
                               !std::is_constructible_v<T, std::string>,
                           void*> = nullptr>
T ConstructFromString(const std::string& value)
{
    throw std::runtime_error{
        "Virtual memory type is not constructible from string"
    };
}

//////////////////////////

template <class ArgType>
struct ParseSingleArgumentVisitor
{
    template <class Tuple>
    static void Call(Tuple& t, const std::vector<std::string>& args,
                     int32_t& index)
    {
        RuntimeGet(
            t, index,
            [args, index](auto& v)
            {
                try
                {
                    v = ConstructFromString<ArgType>(args[index]);
                }
                catch (const boost::bad_lexical_cast& e)
                {
                    throw ArgumentConstructionException{
                        "Failed to convert argument value \"" + args[index] +

                        "\" to virtual memory type - basic arithmetic type (" +
                        std::string{ typeid(MemoryValueType).name() } + ")"
                    };
                }
                catch (const ConstructionFromStringException& e)
                {
                    throw ArgumentConstructionException{
                        "Exception while creating a virtual memory object "
                        "(which is " +
                        std::string{ typeid(MemoryValueType).name() } +
                        ") from string \"" + args[index] +
                        "\": " + std::string{ e.what() }
                    };
                }
            });
        index++;
    };
};

template <class T>
struct ParseArgumentsVisitor
{
    static void Call(commands::CommandCode code, const CommandPtr& obj,
                     const std::vector<std::string>& args)
    {
        if (T::GetTypeCommandCode() == code)
        {
            int32_t index{ 0 };
            typename T::ArgsTupleType tuple;
            cul::typelist::ForEachTemplate<typename T::ArgsTypeList,
                                           ParseSingleArgumentVisitor>::
                template Iterate<typename T::ArgsTupleType>(tuple, args, index);
            obj->SetArguments(tuple);
        }
    }
};

std::string FormattedErrorPlaceInfo(const std::string& file, int32_t line)
{
    return file + ":" + std::to_string(line) + " ";
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

    CommandBuffer result;
    std::string line, token;
    int32_t lineNumber{ 0 };
    while (std::getline(sourceFileStream, line))
    {
        lineNumber++;
        if (line.empty())
        {
            continue;
        }

        token = internal::StripNextWord(line);
        auto commandCodeOpt{ commands::CommandMapping.FindIgnoreCase(token) };
        if (!commandCodeOpt.has_value())
        {
            errStream << internal::FormattedErrorPlaceInfo(path, lineNumber)
                      << "Unknown command : \"" << token << "\"" << std::endl;
            break;
        }
        CommandPtr commandPtr;
        cul::typelist::ForEach<commands::CommandsTypeList,
                               internal::CreateCommandFromCodeVisitor>::
            Iterate(commandCodeOpt.value(), commandPtr);
        std::vector<std::string> args;
        for (int32_t i{ 0 }; i < commandPtr->GetArgumentsNumber(); i++)
        {
            auto argStr{ internal::StripNextWord(line) };
            if (argStr.empty())
            {
                errStream << internal::FormattedErrorPlaceInfo(path, lineNumber)
                          << "Not enough arguments specified for \"" << token
                          << "\" command" << std::endl;
                goto stopLineProcessing;
            }
            args.push_back(argStr);
        }
        if (!line.empty())
        {
            errStream << internal::FormattedErrorPlaceInfo(path, lineNumber)
                      << "Too many arguments specified for \"" + token +
                             "\" command"
                      << std::endl;
            continue;
        }

        try
        {
            cul::typelist::ForEach<commands::CommandsTypeList,
                                   internal::ParseArgumentsVisitor>::
                Iterate(commandCodeOpt.value(), commandPtr, args);
        }
        catch (const ArgumentConstructionException& e)
        {
            errStream << internal::FormattedErrorPlaceInfo(path, lineNumber)
                      << e.what() << std::endl;
            std::shared_ptr<int> a;
            continue;
        }

        result.push_back(commandPtr);

    stopLineProcessing:;
    }

    sourceFileStream.close();
    return result;
}

CommandBuffer FileParser::ParseBinary(const std::string& path,
                                      std::stringstream& errStream)
{
    throw std::logic_error{ "not implemented" };
}

} // namespace cpuemul
