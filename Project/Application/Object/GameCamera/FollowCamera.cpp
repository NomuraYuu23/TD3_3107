#include "FollowCamera.h"
#include "../GameUtility/MathUtility.h"
#include "../ObjectList.h"
#include "../../../Engine/Math/Ease.h"
#include "../../../Engine/GlobalVariables/GlobalVariables.h"

void FollowCamera::Initialize()
{
	// 基底クラス初期化
	BaseCamera::Initialize();

	// 初期値の設定
	defaultOffset_ = GlobalVariables::GetInstance()->GetVector3Value("Camera", "Offset");
	defaultFovY_ = fovY_;

	// ターゲット
	defaultOffsetTarget_ = {10.0f, 0.0f, 0.0f };

	defaultOffset_.y = 30.0f;
	// 補間係数
	targetT_ = 0.5f;

	transform_.rotate.x = 0.3f;

	scalingRate_ = 0.0f;
	scalingRateT_ = 0.2f;

}

void FollowCamera::Update(float elapsedTime)
{
	// 追従処理
	if (targetTransform_) {

		transform_.translate = targetTransform_->transform_.translate + defaultOffset_ + defaultOffsetAdd_;

	}

	// 拡縮処理
	if (player_) {

		ScalingUpDown();
		MoveCameraForward();
	}
	// 基底クラス更新
	BaseCamera::Update(elapsedTime);

}

void FollowCamera::ImGuiDraw()
{

	ImGui::Begin("FollowCamera");

	ImGui::DragFloat3("offset", &defaultOffset_.x, 0.01f, -200.0f, 200.0f);
	Vector3 worldPlayer = player_->worldtransform_.GetWorldPosition();
	ImGui::DragFloat3("playerWorld", &worldPlayer.x);
	Vector2 screenPlayer = MathUtility::WorldToScreen(player_->worldtransform_.GetWorldPosition(), this);
	ImGui::DragFloat2("playerScreen", &screenPlayer.x);


	ImGui::DragFloat("FovY", &fovY_);
	float length = std::sqrtf(std::powf(player_->floorPrevY_ - player_->worldtransform_.GetWorldPosition().y, 2));

	ImGui::DragFloat("Length", &length);

	ImGui::DragFloat3("defaultOffsetTarget_", &defaultOffsetTarget_.x, 0.01f, 0.0f, 20.0f);
	ImGui::DragFloat3("rotate", &transform_.rotate.x, 0.01f, 0.0f, 20.0f);

	ImGui::DragFloat("ScalingRateT", &scalingRateT_, 0.01f, 0.0f, 1.0f);

	ImGui::End();

}

void FollowCamera::ScalingUpDown()
{
	// 距離
	float length = std::sqrtf(std::powf(player_->floorPrevY_ - player_->worldtransform_.GetWorldPosition().y, 2));

	// 距離の最大と最小の値
	float minRange = GlobalVariables::GetInstance()->GetFloatValue("Camera", "MinRange");
	float maxRange = GlobalVariables::GetInstance()->GetFloatValue("Camera", "MaxRange");

	// 新しい長さ
	float newSize = std::clamp(length, minRange, maxRange);

	// 割合計算
	float rate = newSize / maxRange;

	scalingRate_ = Ease::Easing(Ease::EaseName::Lerp, scalingRate_, rate, scalingRateT_);

	// オフセットと引きの最大の値
	Vector3 defaultOff = GlobalVariables::GetInstance()->GetVector3Value("Camera", "Offset");
	float pullMax = GlobalVariables::GetInstance()->GetFloatValue("Camera", "PullOffset");

	// 視野角の最大と最小
	float minFov = GlobalVariables::GetInstance()->GetFloatValue("Camera", "MinFov");
	float maxFov = GlobalVariables::GetInstance()->GetFloatValue("Camera", "MaxFov");

	// 引きカメラの最大
	float maxOffset = defaultOff.z - pullMax;

	// 割合に合わせた値
	nowFovY_ = MathUtility::Ratio(minFov, maxFov, scalingRate_);
	defaultOffset_.z = MathUtility::Ratio(defaultOff.z, maxOffset, scalingRate_);

	SetFovY(nowFovY_);
	// タイマーセット
	float returnTime = 10.0f;
	correctTimer_.Start(returnTime);
}

void FollowCamera::MoveCameraForward()
{

	Input* input = Input::GetInstance();

	// 左
	if (input->GetLeftAnalogstick().x < 0.0f) {
		targetT_ -= 0.01f;
		if (targetT_ < 0.0f) {
			targetT_ = 0.0f;
		}
	}
	// 右
	else if (input->GetLeftAnalogstick().x > 0.0f) {
		targetT_ += 0.01f;
		if (targetT_ > 1.0f) {
			targetT_ = 1.0f;
		}
	}

	defaultOffsetAdd_.x = Ease::Easing(Ease::EaseName::EaseInOutQuart, -defaultOffsetTarget_.x, defaultOffsetTarget_.x, targetT_);

}
