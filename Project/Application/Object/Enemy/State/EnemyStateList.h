#pragma once

#include "IEnemyState.h"

#include "SingleEnemyState.h"
#include "GroupEnemyState.h"
#include "EnemyWaitState.h"
#include "ChaseEnemyState.h"

using EnemyState = std::variant<IEnemyState*, SingleEnemyState*, GroupEnemyState*, EnemyWaitState*, ChaseEnemyState*>;
