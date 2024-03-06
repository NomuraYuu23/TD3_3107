#include "IWeaponState.h"
#include "../../../Engine/Math/Ease.h"

void IWeaponState::PreInitialize(Weapon* weapon)
{
	weapon_ = weapon;
}

void IWeaponState::StartEasing(int frame)
{

	isEnd_ = false;
	easingEndFrame_ = (float)frame;

}

void IWeaponState::EaseUpdate()
{
	// イージング更新
	// フレーム計算
	ease_t_ += (1.0f / easingEndFrame_);
	// 終了条件
	if (ease_t_ >= 1.0f) {
		isEnd_ = true;
		ease_t_ = 1.0f;
	}
}
