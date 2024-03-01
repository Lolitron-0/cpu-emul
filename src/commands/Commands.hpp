#pragma once
#include "AddCommand.hpp"
#include "BeginCommand.hpp"
#include "CallCommand.hpp"
#include "CommandBase.hpp"
#include "DivCommand.hpp"
#include "EndCommand.hpp"
#include "InCommand.hpp"
#include "JaCommand.hpp"
#include "JaeCommand.hpp"
#include "JbCommand.hpp"
#include "JbeCommand.hpp"
#include "JeqCommand.hpp"
#include "JmpCommand.hpp"
#include "JneCommand.hpp"
#include "MulCommand.hpp"
#include "OutCommand.hpp"
#include "OutrCommand.hpp"
#include "PopCommand.hpp"
#include "PoprCommand.hpp"
#include "PushCommand.hpp"
#include "PushrCommand.hpp"
#include "RetCommand.hpp"
#include "SecretCommand.hpp"
#include "SubCommand.hpp"
#include <cul/cul.hpp>

namespace cpuemul
{
namespace commands
{
using Command = CommandBase;

using CommandsTypeList = cul::typelist::TypeList<
    BeginCommand, EndCommand, AddCommand, SubCommand, MulCommand, DivCommand,
    InCommand, OutCommand, OutrCommand, PushCommand, PopCommand, PushrCommand,
    PoprCommand, JmpCommand, JeqCommand, JneCommand, JaCommand, JaeCommand,
    JbCommand, JbeCommand, CallCommand, RetCommand, SecretCommand>;
} // namespace commands
} // namespace cpuemul
