#pragma once

#include "IEnemyState.h"

#include "EnemyAerialState.h"
#include "EnemyGroundState.h"
#include "EnemyWaitState.h"

using EnemyState = std::variant<IEnemyState*, EnemyAerialState*, EnemyGroundState*, EnemyWaitState*>;
