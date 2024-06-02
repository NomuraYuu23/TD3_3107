#include "FreeFallState.h"
#include "../../ObjectList.h"
#include "../../GameUtility/MathUtility.h"
#include "../../../Engine/Input/Input.h"
#include "../../../Engine/Math/DeltaTime.h"

void FreeFallState::Initialize()
{

	// ステート更新
	SetNowState(this);
	// 初速度
	Vector2 power = GlobalVariables::GetInstance()->GetVector2Value("Weapon", "FreeFallInitPower");
	// 速度設定
	// プレイヤーポインタ
	Player* tmpPlayer = weapon_->GetPlayer();

	// ダメージ受けた時
	if (tmpPlayer->GetHitManager().InvisibleActive()) {
		// 向いている方向と逆に
		if (tmpPlayer->worldtransform_.direction_.x > 0) {
			power.x *= -1.0f;
		}
	}
	else {
		// 移動中で右に進んでいるなら左に飛ばす
		if (tmpPlayer->velocity_.x < 0) {
			power.x *= -1.0f;
		}
		// 移動していない場合向いている方向に飛ばす
		else if (tmpPlayer->velocity_.x == 0) {
			if (tmpPlayer->worldtransform_.direction_.x < 0) {
				power.x *= -1.0f;
			}
		}
	}

	// 初速設定
	weapon_->velocity_.x = power.x;
	weapon_->velocity_.y = power.y;

	// 回転速度決定～
	rotateSpinSpeed_ = GlobalVariables::GetInstance()->GetFloatValue("Weapon", "FreeFallSpinSpeed");
	// 重力倍率決定～
	gravityRate_ = GlobalVariables::GetInstance()->GetFloatValue("Weapon", "FreeFallGravityRate");

	// 最初の衝突を回避する時間
	weapon_->attractInvTimer_.Start(1.0f);

	// 所持状態フラグをfalse
	weapon_->isHold_ = false;

	// 武器自体の向きを一定に
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

	// デバッグ以外の場合行う
	#ifdef _RELEASE
	// アイドルアニメーション再生
	weapon_->GetAnimManager()->PlaySpearAnimation(SpearAnimManager::SpearIdle, true);
	#endif // !_DEBUG
}

void FreeFallState::Update()
{

	// 速度処理
	weapon_->velocity_.x = MathUtility::Lerp(weapon_->velocity_.x, 0, 0.01f);
	weapon_->velocity_.y += (kGravity * gravityRate_) * kDeltaTime_ * (1.0f / GameSystemManager::sGameSpeed);

	// 回転
	weapon_->worldtransform_.direction_ = MathUtility::RotateVector(weapon_->worldtransform_.direction_, rotateSpinSpeed_);

	// 座標移動
	weapon_->worldtransform_.transform_.translate += weapon_->velocity_ * kDeltaTime_ * (1.0f / GameSystemManager::sGameSpeed);

}
