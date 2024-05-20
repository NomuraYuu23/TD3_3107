#pragma once

#include "IEnemyState.h"

#include "SingleEnemyState.h"
#include "GroupEnemyState.h"
#include "EnemyWaitState.h"

using EnemyState = std::variant<IEnemyState*, SingleEnemyState*, GroupEnemyState*, EnemyWaitState*>;
