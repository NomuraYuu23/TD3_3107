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
	float valueX = 20.0f;
	// 速度設定
	// weapon_->velocity_ = {};
	//weapon_->velocity_.x = weapon_->velocity_.x * valueX;
	weapon_->velocity_.x = valueX;
	weapon_->velocity_.y = 40.0f;

	// 最初の衝突を回避する時間
	weapon_->attractInvTimer_.Start(1.0f);

	// 所持状態フラグをfalse
	weapon_->isHold_ = false;

	if (weapon_->worldtransform_.direction_.x > 0 && weapon_->worldtransform_.direction_.y > 0) {
		weapon_->worldtransform_.direction_ = Vector3::Normalize({ 1,1,0 });
	}
	else if (weapon_->worldtransform_.direction_.x < 0 && weapon_->worldtransform_.direction_.y > 0) {
		weapon_->worldtransform_.direction_ = Vector3::Normalize({ -1,1,0 });
	}
	else if (weapon_->worldtransform_.direction_.x > 0 && weapon_->worldtransform_.direction_.y < 0) {
		weapon_->worldtransform_.direction_ = Vector3::Normalize({ 1,-1,0 });
	}
	else if (weapon_->worldtransform_.direction_.x < 0 && weapon_->worldtransform_.direction_.y < 0) {
		weapon_->worldtransform_.direction_ = Vector3::Normalize({ -1,-1,0 });
	}
	else {
		weapon_->worldtransform_.direction_ = Vector3::Normalize({ 0,1,0 });
		weapon_->velocity_.y += 15.0f;
	}
	//weapon_->worldtransform_.usedDirection_ = true;
	// デバッグ以外の場合行う
	#ifndef _DEBUG
	// アイドルアニメーション再生
	weapon_->GetAnimManager()->PlaySpearAnimation(SpearAnimManager::SpearIdle, true);
	#endif // !_DEBUG
}

void FreeFallState::Update()
{

	// 速度処理
	weapon_->velocity_.x = MathUtility::Lerp(weapon_->velocity_.x, 0, 0.01f);
	weapon_->velocity_.y += (kGravity * 12.5f) * kDeltaTime_ * (1.0f / GameSystemManager::sGameSpeed);

	// 回転
	float rotateSpeed = 8.0f;
	weapon_->worldtransform_.direction_ = MathUtility::RotateVector(weapon_->worldtransform_.direction_, rotateSpeed);

	// 座標移動
	weapon_->worldtransform_.transform_.translate += weapon_->velocity_ * kDeltaTime_ * (1.0f / GameSystemManager::sGameSpeed);

}
