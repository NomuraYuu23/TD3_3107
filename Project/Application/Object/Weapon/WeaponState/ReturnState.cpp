#include "ReturnState.h"
#include "../Weapon.h"
#include "../../../Engine/Input/Input.h"
#include "../../../Engine/Math/Ease.h"
#include "../../../Engine/Math/Math.h"
#include "../../GameUtility/MathUtility.h"

void ReturnState::Initialize()
{
	SetNowState(this);

	// 角度から方向
	weapon_->worldtransform_.usedDirection_ = true;

	//StartEasing(30);
	startPosition_ = weapon_->worldtransform_.transform_.translate;
	lerp_t = 0.2f;
	CreateRelayPoint();
}

void ReturnState::Update()
{
	// 終了処理
	if (isEnd_) {
		weapon_->ChangeRequest(Weapon::StateName::kHold);
		//weapon_->ChangeState(std::make_unique<HoldState>());
		return;
	}

	// イージング更新
	EaseUpdate();

}

void ReturnState::ImGuiUpdate()
{
	ImGui::Begin("ReturnState");

	ImGui::DragFloat("lerpRatio", &lerp_t, 0.01f, 0, 1);

	ImGui::Separator();

	ImGui::DragFloat3("RelayPos", &relayPosition_.x);
	Vector3 tPos = weapon_->GetTargetPosition();
	ImGui::DragFloat3("tagetPos", &tPos.x);
	Vector3 wPos = weapon_->worldtransform_.GetWorldPosition();
	ImGui::DragFloat3("weaponPos", &wPos.x);

	ImGui::Text("%d : isRelay", RelayTimer_.GetNowFrame());

	ImGui::End();

}

void ReturnState::EaseUpdate()
{
	// 中継座標移動のタイマー
	RelayTimer_.Update();

	// 線形補間による座標更新
	if (RelayTimer_.IsEnd()) {
		// 座標
		weapon_->worldtransform_.transform_.translate.x = MathUtility::Lerp(weapon_->worldtransform_.transform_.translate.x, weapon_->GetTargetPosition().x, lerp_t);
		weapon_->worldtransform_.transform_.translate.y = MathUtility::Lerp(weapon_->worldtransform_.transform_.translate.y, weapon_->GetTargetPosition().y, lerp_t);
		// 角度
		weapon_->worldtransform_.direction_ = weapon_->worldtransform_.transform_.translate - weapon_->GetTargetPosition();
	}
	else {
		// 座標
		weapon_->worldtransform_.transform_.translate.x = MathUtility::Lerp(weapon_->worldtransform_.transform_.translate.x, relayPosition_.x, 0.02f);
		weapon_->worldtransform_.transform_.translate.y = MathUtility::Lerp(weapon_->worldtransform_.transform_.translate.y, relayPosition_.y, 0.02f);
		// 方向ベクトル
		weapon_->worldtransform_.direction_ = weapon_->worldtransform_.transform_.translate - Vector3(relayPosition_.x, relayPosition_.y, 0);
	}

}

void ReturnState::CreateRelayPoint()
{
	// Y座標のオフセット
	float offSet = 30.0f;
	// 中継地点に向けた動きのフレーム数
	float relayFrame = 30.0f;
	float rate = weapon_->GetReturnRate();
	// 上から下に向けた戻り
	if (weapon_->returnDirect_.y > 0) {
		// 中継座標のX座標
		//float relay_x = Math::Lerp(weapon_->worldtransform_.GetWorldPosition().x, weapon_->GetTargetPosition().x, 0.9f);
		float relay_x = MathUtility::Lerp(weapon_->worldtransform_.GetWorldPosition().x, weapon_->GetTargetPosition().x, rate);
		// 中継座標
		relayPosition_ = { relay_x,weapon_->GetTargetPosition().y + offSet,0 };
		RelayTimer_.Start(relayFrame);
	}
	// 下から上に向けた戻り
	else if (weapon_->returnDirect_.y < 0) {
		// 中継座標のX座標
		//float relay_x = Math::Lerp(weapon_->worldtransform_.GetWorldPosition().x, weapon_->GetTargetPosition().x, 0.9f);
		float relay_x = MathUtility::Lerp(weapon_->worldtransform_.GetWorldPosition().x, weapon_->GetTargetPosition().x, rate);
		// 中継座標
		relayPosition_ = { relay_x,weapon_->GetTargetPosition().y - offSet,0 };
		RelayTimer_.Start(relayFrame);
	}
	// 入力なしのニュートラル
	else {
		RelayTimer_.Start(1.0f);
	}

}
