#pragma once
#include "AddCommand.hpp"
#include "BeginCommand.hpp"
#include "CommandBase.hpp"
#include "DivCommand.hpp"
#include "EndCommand.hpp"
#include "InCommand.hpp"
#include "MulCommand.hpp"
#include "OutCommand.hpp"
#include "PopCommand.hpp"
#include "PushCommand.hpp"
#include "SubCommand.hpp"
#include <cul/cul.hpp>

namespace cpuemul
{
namespace commands
{
using Command = CommandBase;

using CommandsTypeList =
    cul::typelist::TypeList<BeginCommand, EndCommand, AddCommand, SubCommand, MulCommand,
                  DivCommand, InCommand, OutCommand, PushCommand, PopCommand>;
} // namespace commands
} // namespace cpuemul
