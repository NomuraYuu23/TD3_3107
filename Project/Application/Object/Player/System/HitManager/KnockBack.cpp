#include "KnockBack.h"
#include "../../Player.h"
#include "../../../Engine/Math/Ease.h"
#include "../../../GameUtility/MathUtility.h"
#include "../../../Engine/GlobalVariables/GlobalVariables.h"

void KnockBack::Initialize(Player* player)
{
	player_ = player;
	knockBackPower_ = { 40.0f,40.0f,0 };
	// フラグ
	isHit_ = false;
}

void KnockBack::CreateKnockBack(const Vector3& direction)
{
	// 衝突したタイミングでtrue、着地でfalse;
	isHit_ = true;
	// 方向
	moveDirection_ = Vector3::Normalize(direction);
	// 上に矯正
	if (moveDirection_.y < 0) {
		moveDirection_.y *= -1.0f;
	}
	// 速度
	player_->velocity_ = { moveDirection_.x * knockBackPower_.x,moveDirection_.y * knockBackPower_.y,0 };

	AcceptTimer_.Start(30.0f);
}

void KnockBack::Update()
{
	if (!AcceptTimer_.IsActive()) {
		return;
	}

	// 移動ベクトルの処理
	// 反動の速度ベクトルを計算	
	float ratio = 0.005f;
	player_->velocity_.x = MathUtility::Lerp(player_->velocity_.x, 0, ratio);
	player_->velocity_.y = MathUtility::Lerp(player_->velocity_.y, 0, ratio);

	AcceptTimer_.Update();
}
