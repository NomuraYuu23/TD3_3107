#include "IWeaponState.h"
#include "../Weapon.h"
#include "../../../Engine/Math/Ease.h"

void IWeaponState::PreInitialize(Weapon* weapon)
{
	weapon_ = weapon;
}

void IWeaponState::StartEasing(int frame)
{
	// 開始フラグ
	isStart_ = true;
	// 終了フラグ
	isEnd_ = false;
	// 終了フレーム
	easingEndFrame_ = (float)frame;

}

void IWeaponState::EaseUpdate()
{
	// イージング更新
	// フレーム計算
	if (isStart_) {
		ease_t_ += (1.0f / easingEndFrame_);
		// 終了条件
		if (ease_t_ >= 1.0f) {
			isEnd_ = true;
			isStart_ = false;
			ease_t_ = 1.0f;
		}
	}
}

void IWeaponState::SetNowState(WeaponState state)
{
	// 切り替わるタイミングで呼ぶ
	// 現在のステートの設定
	weapon_->nowState_ = state;
}
