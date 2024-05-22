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
}

void FollowCamera::Update(float elapsedTime)
{
	// 追従処理
	if (targetTransform_) {

		transform_.translate = targetTransform_->transform_.translate + defaultOffset_;

	}

	// 拡縮処理
	if (player_) {

		ScalingUpDown();
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

	// オフセットと引きの最大の値
	Vector3 defaultOff = GlobalVariables::GetInstance()->GetVector3Value("Camera", "Offset");
	float pullMax = GlobalVariables::GetInstance()->GetFloatValue("Camera", "PullOffset");

	// 視野角の最大と最小
	float minFov = GlobalVariables::GetInstance()->GetFloatValue("Camera", "MinFov");
	float maxFov = GlobalVariables::GetInstance()->GetFloatValue("Camera", "MaxFov");

	// 引きカメラの最大
	float maxOffset = defaultOff.z - pullMax;

	// 距離が最低より短い場合
	if (minRange > length) {
		nowFovY_ = minFov;
		defaultOffset_.z = defaultOff.z;
	}
	// それより長い
	else {
		// 割合に合わせた値
		nowFovY_ = MathUtility::Ratio(minFov, maxFov, rate);
		defaultOffset_.z = MathUtility::Ratio(defaultOff.z, maxOffset, rate);
	}

	SetFovY(nowFovY_);
	// タイマーセット
	float returnTime = 10.0f;
	correctTimer_.Start(returnTime);
}
