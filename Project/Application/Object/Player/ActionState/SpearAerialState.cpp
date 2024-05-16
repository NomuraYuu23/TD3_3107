#include "SpearAerialState.h"
#include "../Player.h"
#include "../../GameUtility/MathUtility.h"

void SpearAerialState::Initialize()
{

	// 落下処理
	// jsonデータ
	const char* groupName = "SpearJump";
	player_->velocity_.y = GlobalVariables::GetInstance()->GetFloatValue(groupName, "SpearJumpPower");

	//groupName = "Common";

	gravity_ = GlobalVariables::GetInstance()->GetFloatValue(groupName, "Gravity");


	player_->SetNowState(this);
	player_->isGround_ = true;
	// コンボ加算
	player_->AddCombo();
	//// キャストして方向設定
	//SpearAerialState* state = dynamic_cast<SpearAerialState*>(player_->actionState_.get());

	Vector2 leftStick = Input::GetInstance()->GetLeftAnalogstick();

	leftStick = { leftStick.x / SHRT_MAX,leftStick.y / SHRT_MAX };

	InitializeDirection(leftStick);


	// 槍ジャンアニメーションの再生
	player_->GetAnimManager()->PlayAnimation(PlayerAnimManager::SpearJump);

	// 槍のバウンドアニメーション再生
	player_->weapon_->GetAnimManager()->PlayAnimation(SpearAnimManager::SpearBounce);

}

void SpearAerialState::Update()
{
	// 速度計算
	float mass = 1.0f;
	float activeRatio = GlobalVariables::GetInstance()->GetFloatValue("Player", "AerialActiveDecelerateRatio");
	float inActiveRatio = GlobalVariables::GetInstance()->GetFloatValue("Player", "AerialInActiveDecelerateRatio");
	Vector2 leftStick = Input::GetInstance()->GetLeftAnalogstick();
	// X速度
	if (leftStick.x != 0) {
		player_->velocity_.x = MathUtility::Lerp(player_->velocity_.x, 0, activeRatio);
	}
	else {
		player_->velocity_.x = MathUtility::Lerp(player_->velocity_.x, 0, inActiveRatio);
	}

	// Y速度
	if (player_->IsNowAssistDash()) {
		float ratio = GlobalVariables::GetInstance()->GetFloatValue("Dash", "SlowRatio");
		if (player_->velocity_.y < 0) {
			player_->velocity_.y += mass * (kGravity * (gravity_ / ratio)) * kDeltaTime_ * (1.0f / IObject::sPlaySpeed);
		}
		else {
			player_->velocity_.y += mass * (kGravity * gravity_) * kDeltaTime_ * (1.0f / IObject::sPlaySpeed);
		}
	}
	else {
		player_->velocity_.y += mass * (kGravity * gravity_) * kDeltaTime_ * (1.0f / IObject::sPlaySpeed);
	}

	// 移動処理
	player_->worldtransform_.transform_.translate.x += (player_->velocity_.x) * kDeltaTime_ * (1.0f / IObject::sPlaySpeed);
	player_->worldtransform_.transform_.translate.y += (player_->velocity_.y) * kDeltaTime_ * (1.0f / IObject::sPlaySpeed);
}

void SpearAerialState::InitializeDirection(const Vector2& direct)
{
	if (direct.x > 0) {
		player_->velocity_.x = CreateNewSpeed(direct.x);
	}
	else if (direct.x < 0) {
		player_->velocity_.x = CreateNewSpeed(direct.x);
	}
	else {
		player_->velocity_.x = 0;
	}
}
