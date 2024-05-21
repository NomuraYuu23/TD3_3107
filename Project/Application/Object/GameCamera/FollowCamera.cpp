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
	minY = 1.0f;
	maxY = 30.0f;
	defaultFovY_ = fovY_;
	pullMax_ = 15.0f;
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

	ImGui::DragFloat("Min", &minY, 0.01f, 0, 20.0f);
	ImGui::DragFloat("Max", &maxY, 0.01f, 0, 100.0f);

	ImGui::DragFloat("FovY", &fovY_);
	float length = std::sqrtf(std::powf(player_->floorPrevY_ - player_->worldtransform_.GetWorldPosition().y, 2));
	float newSize = std::clamp(length, minY, maxY);

	ImGui::DragFloat("Length", &length);
	ImGui::DragFloat("size", &newSize);
	ImGui::DragFloat("PullSize", &pullMax_, 0.01f);
	ImGui::End();

}

void FollowCamera::ScalingUpDown()
{
	float length = std::sqrtf(std::powf(player_->floorPrevY_ - player_->worldtransform_.GetWorldPosition().y, 2));
	float newSize = std::clamp(length, minY, maxY);

	// 割合計算
	float rate = newSize / maxY;

	Vector3 defaultOff = GlobalVariables::GetInstance()->GetVector3Value("Camera", "Offset");

	float maxOffset = defaultOff.z - pullMax_;

	if (minY > length) {
		nowFovY_ = 0.45f;
		defaultOffset_.z = defaultOff.z;
	}
	else {
		nowFovY_ = MathUtility::Ratio(0.45f, 0.55f, rate);
		defaultOffset_.z = MathUtility::Ratio(defaultOff.z, maxOffset, rate);
	}

	SetFovY(nowFovY_);
	// タイマーセット
	float returnTime = 10.0f;
	correctTimer_.Start(returnTime);
}
