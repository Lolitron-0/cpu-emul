#include "RuntimeContext.hpp"
#include "FileParser.hpp"

namespace cpuemul
{

std::unordered_map<std::string, size_t> LabelTag::Mapping{};

RegisterNameWrapper::RegisterNameWrapper(const std::string& str)
{
    auto opt{ internal::RegisterNameMapping.FindIgnoreCase(str) };
    if (!opt.has_value())
    {
        throw ConstructionFromStringException{ "No such register \"" + str +
                                               "\"" };
    }
    value = opt.value();
}

}; // namespace cpuemul
