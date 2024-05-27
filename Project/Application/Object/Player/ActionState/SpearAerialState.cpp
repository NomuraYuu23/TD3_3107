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

	player_->weapon_->GetEffectSystem()->StartShockWave(15.0f);

#ifndef _DEBUG

	// プレイヤーの移動ベクトルの逆ベクトルを求める
	Vector3 v = Vector3::Normalize(player_->velocity_);
	v *= -1.0f;

	// 槍ジャンプパーティクル再生
	EmitterDesc desc;
	desc.transform = &player_->worldtransform_.transform_;
	desc.instanceCount = 5;
	desc.frequency = 0.01f;
	desc.lifeTime = 0.025f;
	desc.particleModelNum = kCircle;
	desc.velocity = v;
	desc.paeticleName = kSpearJumpParticle;

	ParticleManager::GetInstance()->MakeEmitter(&desc, 0);

	// 槍ジャンアニメーションの再生
	player_->GetAnimManager()->PlaySpearAnimation(PlayerAnimManager::SpearJump);

	// 槍のバウンドアニメーション再生
	player_->weapon_->GetAnimManager()->PlaySpearAnimation(SpearAnimManager::SpearBounce);

	// ジャンプ効果音を再生
	player_->gameAudioManager_->PlayWave(GameAudioNameIndex::kPlayerJump);

#endif // !_DEBUG

	// 補正キャンセル
	player_->GetLandingAdjuster().Cancel();

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
			player_->velocity_.y += mass * (kGravity * (gravity_ / ratio)) * kDeltaTime_ * (1.0f / GameSystemManager::sGameSpeed);
		}
		else {
			player_->velocity_.y += mass * (kGravity * gravity_) * kDeltaTime_ * (1.0f / GameSystemManager::sGameSpeed);
		}
	}
	else {
		player_->velocity_.y += mass * (kGravity * gravity_) * kDeltaTime_ * (1.0f / GameSystemManager::sGameSpeed);
	}

	// 移動処理
	player_->worldtransform_.transform_.translate.x += (player_->velocity_.x) * kDeltaTime_ * (1.0f / GameSystemManager::sGameSpeed);
	player_->worldtransform_.transform_.translate.y += (player_->velocity_.y) * kDeltaTime_ * (1.0f / GameSystemManager::sGameSpeed);
}

void SpearAerialState::InitializeDirection(const Vector2& direct)
{
	if (direct.x > 0) {
		float moveDirect = 1.0f;
		player_->velocity_.x = CreateNewSpeed(moveDirect);
	}
	else if (direct.x < 0) {
		float moveDirect = -1.0f;
		player_->velocity_.x = CreateNewSpeed(moveDirect);
	}
	else {
		player_->velocity_.x = 0;
	}
}
