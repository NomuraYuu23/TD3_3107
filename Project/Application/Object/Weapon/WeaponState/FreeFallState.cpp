#include "FreeFallState.h"
#include "../../Weapon/Weapon.h"
#include "../../GameUtility/MathUtility.h"
#include "../../../Engine/Input/Input.h"
#include "../../../Engine/Math/DeltaTime.h"

void FreeFallState::Initialize()
{

	// ステート更新
	SetNowState(this);
	// 仮の値
	float valueX = 2.0f;
	// 速度設定
	// weapon_->velocity_ = {};
	weapon_->velocity_.x = weapon_->velocity_.x * valueX;
	weapon_->velocity_.y = 40.0f;

	// 最初の衝突を回避する時間
	weapon_->attractInvTimer_.Start(5.0f);
}

void FreeFallState::Update()
{

	// 速度処理
	weapon_->velocity_.x = MathUtility::Lerp(weapon_->velocity_.x, 0, 0.01f);
	weapon_->velocity_.y += (kGravity * 12.5f) * kDeltaTime_ * (1.0f / IObject::sPlaySpeed);

	// 回転
	weapon_->worldtransform_.direction_ = MathUtility::RotateVector(weapon_->worldtransform_.direction_, (3.14f / 24.0f));

	// 座標移動
	weapon_->worldtransform_.transform_.translate += weapon_->velocity_ * kDeltaTime_ * (1.0f / IObject::sPlaySpeed);

}
