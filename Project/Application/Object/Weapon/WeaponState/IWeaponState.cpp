#include "IWeaponState.h"
#include "../Weapon.h"
#include "../../../Engine/Math/Ease.h"

void IWeaponState::PreInitialize(Weapon* weapon)
{
	weapon_ = weapon;
}

void IWeaponState::SetNowState(WeaponState state)
{
	// 切り替わるタイミングで呼ぶ
	// 現在のステートの設定
	weapon_->nowState_ = state;
}
