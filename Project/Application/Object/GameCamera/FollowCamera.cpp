#include "FollowCamera.h"
#include "../GameUtility/MathUtility.h"

void FollowCamera::Initialize()
{
	// 基底クラス初期化
	BaseCamera::Initialize();

	defaultOffset_ = { 0,0,-85.0f };

}

void FollowCamera::Update(float elapsedTime)
{
	// 追従処理
	if (targetTransform_) {

		transform_.translate = targetTransform_->transform_.translate + defaultOffset_;
		// X,Z座標
		transform_.translate.x = targetTransform_->transform_.translate.x + defaultOffset_.x;
		transform_.translate.z = targetTransform_->transform_.translate.z + defaultOffset_.z;
	
		//Vector2 screenPosition = MathUtility::WorldToScreen(player_->worldtransform_.GetWorldPosition(), this);

		//if (screenPosition.y > 330) {
		//	transform_.translate.y = 0;
		//}
		//else {
		//	transform_.translate.y = targetTransform_->transform_.translate.y + defaultOffset_.y;
		//}

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

	ImGui::End();

}
