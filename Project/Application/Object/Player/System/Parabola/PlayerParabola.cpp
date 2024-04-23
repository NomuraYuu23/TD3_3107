#include "PlayerParabola.h"
#include "../../../../../Engine/GlobalVariables/GlobalVariables.h"
#include "../../../../../Engine/Physics/massPoint.h"
#include "../../../../../Engine/Math/DeltaTime.h"
#include "../../Player.h"

void PlayerParabola::Initialize()
{

	// 線
	for (uint32_t i = 0; i < kNumLineMax_; ++i) {
		drawLines_[i].reset(DrawLine::Create());
		startPositions_[i] = { 0.0f,0.0f,0.0f };
		endPositions_[i] = { 0.0f,0.0f,0.0f };
	}

	// 線の数
	numLine_ = 0;

	// 速度
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	speed_ = globalVariables->GetFloatValue("Weapon", "SpeedRatio");

}

void PlayerParabola::Update(const Vector3& playerPosition, const Vector3& direct, Player* player)
{

	// 初速
	Vector3 velocity = direct;
	velocity *= speed_;

	// 加速度
	Vector3 acceleration = { 0.0f,0.0f,0.0f };
	float gravity = GlobalVariables::GetInstance()->GetFloatValue("Weapon", "Gravity");
	// 重力倍率
	const uint32_t kGravityMagnification = 50;

	if (std::holds_alternative<GroundState*>(player->GetNowState())) {
		//acceleration.y = (-9.8f * gravity * kGravityMagnification) * kDeltaTime_;
	}


	// 線の数をリセット
	numLine_ = 0;

	// 最初の位置を設定
	startPositions_[0] = playerPosition;
	for (uint32_t i = 0; i < kNumLineMax_; ++i) {

		// 速度
		velocity =  MassPoint::VelocityCalc(velocity, acceleration, kDeltaTime_ * kTimeMagnification);
		// 終了位置
		endPositions_[i] = MassPoint::PositionCalc(startPositions_[i], velocity, kDeltaTime_ * kTimeMagnification);
		
		// スタート位置
		if (i != kNumLineMax_ - 1) {
			startPositions_[i + 1] = endPositions_[i];
		}
		// 線の数を追加
		numLine_++;

		// ループを終了するか確認
		// 地面についたか
		if (endPositions_[i].y <= -2.5f) {

			float t = std::fabsf(-2.5f - startPositions_[i].y) / std::fabsf(endPositions_[i].y - startPositions_[i].y);

			endPositions_[i] = (endPositions_[i] - startPositions_[i]) * t + startPositions_[i];
			break;
		}
	}

}

void PlayerParabola::Reset()
{

	numLine_ = 0;

}

void PlayerParabola::Draw(BaseCamera& camera)
{

	// 色
	Vector4 color = { 0.0f,0.8f,0.4f,1.0f };

	// 描画
	for (uint32_t i = 0; i < numLine_; ++i) {
		drawLines_[i]->Draw(
			startPositions_[i],
			endPositions_[i],
			color,
			color,
			camera);
	}

}
