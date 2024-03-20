#pragma once

///
///	ステートのIncludePath
///
#include "IActionState.h"
#include "GroundState.h"
#include "AerialState.h"
#include "ActionWaitState.h"
#include "SpearAerialState.h"

// ステートの名前
using PlayerState = std::variant<IActionState*, GroundState*, AerialState*, ActionWaitState*, SpearAerialState*>;