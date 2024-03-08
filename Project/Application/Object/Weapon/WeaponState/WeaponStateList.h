#pragma once
///
///	武器ステートのIncludePath
/// 
// 基底クラス
#include "IWeaponState.h"

// 手で持っている状態
#include "HoldState.h"
// 投げられている状態（空中
#include "ThrownState.h"
// 刺さってる状態
#include "ImpaledState.h"
// 帰ってきてる状態（空中
#include "ReturnState.h"
