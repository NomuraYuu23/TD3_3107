#include "ImpaledState.h"
#include "../Weapon.h"

void ImpaledState::Initialize()
{
	// 終了までのカウント開始
	//StartEasing(120);
	SetNowState(this);
	// 親子関係を解消し、ワールド座標を適応
	weapon_->worldtransform_.transform_.translate = weapon_->worldtransform_.GetWorldPosition();
	weapon_->worldtransform_.parent_ = nullptr;
	weapon_->worldtransform_.UpdateMatrix();

	// デバッグ以外の場合行う
	#ifdef _RELEASE

	// 逆ベクトルを求める
	Vector3 v = {
		 -weapon_->worldtransform_.direction_.x,
		 -weapon_->worldtransform_.direction_.y,
		 -weapon_->worldtransform_.direction_.z };

	// 槍が刺さっている方向ベクトルの正規化
	Vector3 d = Vector3::Normalize(weapon_->worldtransform_.direction_);

	// 生成時トランスフォームの生成
	EulerTransform emitT =
	{
		{1.25f, 2.5f, 1.0f},
		{0.0f, 0.0f, 0.0f},
		{ weapon_->worldtransform_.transform_.translate.x + d.x,
		  weapon_->worldtransform_.transform_.translate.y + d.y,
		  weapon_->worldtransform_.transform_.translate.z + d.z }
	};

	// 槍刺さったときのパーティクル再生
	EmitterDesc desc;
	desc.transform = &emitT;
	desc.instanceCount = 5;
	desc.frequency = 0.01f;
	desc.lifeTime = 0.01f;
	desc.velocity = v;
	desc.particleModelNum = kCircle;
	desc.paeticleName = kImpaledParticle;

	ParticleManager::GetInstance()->MakeEmitter(&desc, 0);

	// 待機アニメーション開始
	weapon_->GetAnimManager()->PlaySpearAnimation(SpearAnimManager::SpearIdle, true);
	#endif // !_DEBUG
}

void ImpaledState::Update()
{
	// 変更処理
	if (isEnd_) {
		weapon_->ChangeRequest(Weapon::StateName::kReturn);
		return;
	}

}

void ImpaledState::ImGuiUpdate()
{

	ImGui::Begin("Impaled");

	if (ImGui::Button("ReturnButton")) {
		isEnd_ = true;
	}

	ImGui::Separator();

	ImGui::DragInt("easeTime", &easeTime_, 1, 1, 600);

	ImGui::End();

}
