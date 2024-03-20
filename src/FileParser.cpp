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
    auto lastSpace{ spacePos };
    while (lastSpace + 1 < str.length() && str[lastSpace + 1] == ' ')
    {
        lastSpace++;
    }

    auto token{ str.substr(0, spacePos) };
    str.erase(0, lastSpace + 1);
    return token;
}

template <class T>
struct CreateCommandFromCodeVisitor
{
    static void Call(commands::CommandCode code, CommandPtr& result)
    {
        if (T::Code == code)
        {
            result = std::make_shared<T>();
        }
    }
};

template <class T>
struct ArgNumFromCodeVisitor
{
    static void Call(commands::CommandCode code, uint32_t& result)
    {
        if (T::Code == code)
        {
            // result = T::;
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
        if (T::Code == code)
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

/////////////////////////////////////////////

template <class T>
void Serialize(std::fstream& out, T);

template <class T>
void Serialize(
    std::fstream& out,
    std::enable_if_t<std::is_arithmetic_v<T> || std::is_enum_v<T>, T> obj)
{
    out.write(reinterpret_cast<char*>(&obj), sizeof(obj));
}

template <>
void Serialize<std::string>(std::fstream& out, std::string str)
{
    auto size{ str.size() };
    out.write(reinterpret_cast<char*>(&size), sizeof(size));
    out.write(str.c_str(), size);
}

template <>
void Serialize<RegisterNameWrapper>(std::fstream& out, RegisterNameWrapper reg)
{
    out.write(reinterpret_cast<char*>(&reg.value), sizeof(reg.value));
}

template <>
void Serialize<LabelTag>(std::fstream& out, LabelTag reg)
{
    out.write(reinterpret_cast<char*>(&reg.value), sizeof(reg.value));
}

template <class ArgType>
struct SerializeSingleArgumentVisitor
{
    static void Call(std::fstream& out, const std::vector<std::string>& args,
                     int32_t& index)
    {
        // noexcept
        auto v = ConstructFromString<ArgType>(args[index]);
        Serialize<ArgType>(out, v);
        index++;
    }
};

template <class T>
struct SerializeArgumentsVisitor
{
    static void Call(commands::CommandCode code, std::fstream& out,
                     const std::vector<std::string>& args)
    {
        if (T::Code == code)
        {
            int32_t index{ 0 };
            cul::typelist::ForEach<
                typename T::ArgsTypeList,
                SerializeSingleArgumentVisitor>::Iterate(out, args, index);
        }
    }
};

////////////////////////////////////

template <class T>
std::enable_if_t<!std::is_arithmetic_v<T> && !std::is_enum_v<T>, T>
Deserialize(std::fstream& out);

template <class T>
std::enable_if_t<std::is_arithmetic_v<T> || std::is_enum_v<T>, T>
Deserialize(std::fstream& out)
{
    T obj;
    out.read(reinterpret_cast<char*>(&obj), sizeof(obj));
    return obj;
}

template <>
std::string Deserialize<std::string>(std::fstream& out)
{
    size_t size;
    std::string str;
    str.resize(size);
    out.read(reinterpret_cast<char*>(&size), sizeof(size));
    char* buf = new char[size + 1];
    out.read(buf, size);
    buf[size] = '\0';
    str = buf;
    delete[] buf;
    return str;
}

template <>
RegisterNameWrapper Deserialize<RegisterNameWrapper>(std::fstream& out)
{
    RegisterNameWrapper reg;
    out.read(reinterpret_cast<char*>(&reg.value), sizeof(reg.value));
    return reg;
}

template <>
LabelTag Deserialize<LabelTag>(std::fstream& out)
{
    LabelTag tag;
    out.read(reinterpret_cast<char*>(&tag.value), sizeof(tag.value));
    return tag;
}

template <class ArgType>
struct DeserializeSingleArgumentVisitor
{
    template <class Tuple>
    static void Call(Tuple& t, std::fstream& bin, int32_t& index)
    {
        RuntimeGet(t, index,
                   [&bin, index](auto& v) { v = Deserialize<ArgType>(bin); });
        index++;
    }
};

template <class T>
struct DeserializeArgumentVisitor
{
    static void Call(commands::CommandCode code, const CommandPtr& obj,
                     std::fstream& bin)
    {
        if (T::Code == code)
        {
            int32_t index{ 0 };
            typename T::ArgsTupleType tuple;
            cul::typelist::ForEachTemplate<typename T::ArgsTypeList,
                                           DeserializeSingleArgumentVisitor>::
                template Iterate<typename T::ArgsTupleType>(tuple, bin, index);
            obj->SetArguments(tuple);
        }
    }
};

std::string FormattedErrorPlaceInfo(const std::string& file, int32_t line)
{
    return file + ":" + std::to_string(line) + " ";
}

} // namespace internal

std::vector<std::string>
FileParser::CleanSourceFile(const std::string& path,
                            std::stringstream& errStream)
{
    std::vector<std::string> result{};

    std::ifstream sourceFileStream{ path };
    if (!sourceFileStream.is_open())
    {
        errStream << "File not found: " << path;
        return result;
    }

    int32_t lineNumber{ 0 };
    std::string line, lineCopy, token;
    int32_t commandIdx{ 0 };
    while (std::getline(sourceFileStream, line))
    {
        lineCopy = line;
        lineNumber++;
        if (line.empty())
        {
            continue;
        }

        token = internal::StripNextWord(line);

        if (token == "//")
        {
            continue;
        }

        auto commandCodeOpt{ commands::CommandMapping.FindIgnoreCase(token) };
        if (!commandCodeOpt.has_value())
        {
            if (token.size() >= 2 && token.ends_with(':')) // label
            {
                result.push_back(token);
                LabelTag::Mapping[token.substr(0, token.size() - 1)] =
                    commandIdx;
                continue;
            }

            errStream << internal::FormattedErrorPlaceInfo(path, lineNumber)
                      << "Unknown token : \"" << token << "\"" << std::endl;
            continue;
        }
        // TODO: no creation foreach
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

        result.push_back(lineCopy);
        commandIdx++;
    stopLineProcessing:;
    }

    sourceFileStream.close();
    return result;
}

std::tuple<CommandBuffer, LabelMap>
FileParser::ParseSourceFile(const std::string& path,
                            std::stringstream& errStream,
                            const std::string& binaryExportPath)
{
    CommandBuffer resultCommands;
    LabelMap resultLabels;
    std::string token;
    int32_t lineNumber{ 0 };
    auto lines{ CleanSourceFile(path, errStream) };
    if (errStream.rdbuf()->in_avail())
    {
        return {};
    }

    bool shouldExport{ !binaryExportPath.empty() };
    std::fstream binaryFileStream;

    if (shouldExport)
    {
        binaryFileStream.open(binaryExportPath,
                              std::ios::out | std::ios::binary);
    }

    for (auto& line : lines)
    {
        lineNumber++;
        token = internal::StripNextWord(line);

        auto commandCodeOpt{ commands::CommandMapping.FindIgnoreCase(token) };
        if (!commandCodeOpt.has_value()) // label
        {
            resultLabels[atoi(token.substr(0, token.size() - 1).c_str())] =
                resultCommands.size();
            continue;
        }
        CommandPtr commandPtr;
        cul::typelist::ForEach<commands::CommandsTypeList,
                               internal::CreateCommandFromCodeVisitor>::
            Iterate(commandCodeOpt.value(), commandPtr);

        std::vector<std::string> args;
        for (int32_t i{ 0 }; i < commandPtr->GetArgumentsNumber(); i++)
        {
            args.push_back(internal::StripNextWord(line));
        }

        try
        {
            cul::typelist::ForEach<commands::CommandsTypeList,
                                   internal::ParseArgumentsVisitor>::
                Iterate(commandCodeOpt.value(), commandPtr, args);
            if (shouldExport)
            {
                auto commandCode{ commandPtr->GetCommandCode() };
                binaryFileStream.write(reinterpret_cast<char*>(&commandCode),
                                       sizeof(commandCode));
                cul::typelist::ForEach<commands::CommandsTypeList,
                                       internal::SerializeArgumentsVisitor>::
                    Iterate(commandCodeOpt.value(), binaryFileStream, args);
            }
        }
        catch (const ArgumentConstructionException& e)
        {
            errStream << internal::FormattedErrorPlaceInfo(path, lineNumber)
                      << e.what() << std::endl;
            continue;
        }

        resultCommands.push_back(commandPtr);
    }

    if (shouldExport)
    {
        // for (auto pair : resultLabels)
        // {
        //     int8_t labelToken{ -1 };
        //     binaryFileStream.write(reinterpret_cast<char*>(&labelToken),
        //                            sizeof(labelToken));
        //     binaryFileStream.write(reinterpret_cast<const
        //     char*>(&pair.first),
        //                            sizeof(pair.first));
        //     binaryFileStream.write(reinterpret_cast<const
        //     char*>(&pair.second),
        //                            sizeof(pair.second));
        // }
        binaryFileStream.close();
    }
    return std::make_tuple(resultCommands, resultLabels);
}

std::tuple<CommandBuffer, LabelMap>
FileParser::ParseBinary(const std::string& path, std::stringstream& errStream)
{
    CommandBuffer resultCommands;
    LabelMap resultLabels;
    std::fstream binaryFileStream;
    binaryFileStream.open(path, std::ios::in | std::ios::binary);
    while (binaryFileStream.rdbuf()->in_avail())
    {
        int8_t code;
        binaryFileStream.read(reinterpret_cast<char*>(&code), sizeof(code));
        //      if (code == -1) // label
        //      {
        //          LabelMap::key_type labelNum;
        //          binaryFileStream.read(reinterpret_cast<char*>(&labelNum),
        //                                 sizeof(labelNum));
        //          LabelMap::mapped_type labelPos;
        //          binaryFileStream.read(reinterpret_cast<char*>(&labelPos),
        //                                 sizeof(labelPos));
        //          resultLabels[labelNum] = labelPos;
        // continue;
        //      }
        CommandPtr commandPtr;
        cul::typelist::ForEach<commands::CommandsTypeList,
                               internal::CreateCommandFromCodeVisitor>::
            Iterate(static_cast<commands::CommandCode>(code), commandPtr);
        cul::typelist::ForEach<commands::CommandsTypeList,
                               internal::DeserializeArgumentVisitor>::
            Iterate(static_cast<commands::CommandCode>(code), commandPtr,
                    binaryFileStream);
        resultCommands.push_back(commandPtr);
    }
    binaryFileStream.close();
    return std::make_pair(resultCommands, resultLabels);
}

} // namespace cpuemul
