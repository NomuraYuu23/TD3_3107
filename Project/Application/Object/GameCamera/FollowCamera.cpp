#include "FollowCamera.h"
#include "../GameUtility/MathUtility.h"
#include "../ObjectList.h"
#include "../../../Engine/Math/Ease.h"

void FollowCamera::Initialize()
{
	// 基底クラス初期化
	BaseCamera::Initialize();

	// 初期値の設定
	defaultOffset_ = { 0,5.0f,-85.0f };
	minY = 1.0f;
	maxY = 30.0f;
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
		float length = std::sqrtf(std::powf(player_->floorPrevY_ - player_->worldtransform_.GetWorldPosition().y, 2));
		float newSize = std::clamp(length, minY, maxY);
		//if (/*!std::holds_alternative<GroundState*>(player_->GetNowState())*/ !player_->isGround_) {
			// 割合計算
			float rate = newSize / maxY;
			//rate = std::clamp(rate, 0.3f, 1.0f);
			//nowFovY_ = std::clamp(rate, 0.45f, 0.65f);
			//nowFovY_ = std::clamp(pars, 0.45f, 0.65f);
			if (minY > length) {
				nowFovY_ = 0.45f;
			}
			else {
				nowFovY_ = MathUtility::Ratio(0.45f, 0.55f, rate);
			}

			SetFovY(nowFovY_);
			// タイマーセット
			float returnTime = 10.0f;
			correctTimer_.Start(returnTime);
		//}
		//else {
		//	// 視野角の戻す際に滑らかにする処理
		//	if (correctTimer_.IsActive()) {
		//		float fov = Ease::Easing(Ease::EaseName::Lerp, nowFovY_, defaultFovY_, correctTimer_.GetNowFrame());
		//		SetFovY(fov);
		//	}
		//	// タイマー更新
		//	correctTimer_.Update();

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

	ImGui::DragFloat("Min", &minY, 0.01f, 0, 20.0f);
	ImGui::DragFloat("Max", &maxY, 0.01f, 0, 100.0f);

	ImGui::DragFloat("FovY", &fovY_);
	float length = std::sqrtf(std::powf(player_->floorPrevY_ - player_->worldtransform_.GetWorldPosition().y, 2));
	float newSize = std::clamp(length, minY, maxY);

	ImGui::DragFloat("Length", &length);
	ImGui::DragFloat("size", &newSize);

	ImGui::End();

}
