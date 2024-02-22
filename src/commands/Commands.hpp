#pragma once
#include "AddCommand.hpp"
#include "BeginCommand.hpp"
#include "CommandBase.hpp"
#include "DivCommand.hpp"
#include "EndCommand.hpp"
#include "InCommand.hpp"
#include "MulCommand.hpp"
#include "OutCommand.hpp"
#include "OutrCommand.hpp"
#include "PopCommand.hpp"
#include "PoprCommand.hpp"
#include "PushCommand.hpp"
#include "PushrCommand.hpp"
#include "SubCommand.hpp"
#include <cul/cul.hpp>

namespace cpuemul
{
namespace commands
{
using Command = CommandBase;

using CommandsTypeList =
    cul::typelist::TypeList<BeginCommand, EndCommand, AddCommand, SubCommand,
                            MulCommand, DivCommand, InCommand, OutCommand,
                            OutrCommand, PushCommand, PopCommand, PushrCommand,
                            PoprCommand>;
} // namespace commands
} // namespace cpuemul
