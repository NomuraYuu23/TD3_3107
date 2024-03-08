#pragma once
#include <variant>

// 前方宣言
class IWeaponState;
class HoldState;
class ThrownState;
class ImpaledState;
class ReturnState;

// ステートの名前
using WeaponState = std::variant<IWeaponState*, HoldState*, ThrownState*, ImpaledState*, ReturnState*>;