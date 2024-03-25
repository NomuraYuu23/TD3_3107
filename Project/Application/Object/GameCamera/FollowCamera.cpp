#include "FollowCamera.h"

void FollowCamera::Initialize()
{
	// 基底クラス初期化
	BaseCamera::Initialize();

	defaultOffset_ = { 0,0,-85.0f };

}

void FollowCamera::Update()
{
	// 追従処理
	if (targetTransform_) {

		transform_.translate = targetTransform_->transform_.translate + defaultOffset_;

	}

	// 基底クラス更新
	BaseCamera::Update();

}

void FollowCamera::ImGuiDraw()
{

	ImGui::Begin("FollowCamera");

	ImGui::DragFloat3("offset", &defaultOffset_.x, 0.01f, -200.0f, 200.0f);

	ImGui::End();

}
