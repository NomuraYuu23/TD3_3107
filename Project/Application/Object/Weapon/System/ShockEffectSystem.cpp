#include "ShockEffectSystem.h"
#include "../Weapon.h"

void ShockEffectSystem::Initialize(Weapon* weapon)
{
	weapon_ = weapon;
	// 衝撃波マネージャの初期化
	shockWaveManager_ = std::make_unique<ShockWaveManager>();
	shockWaveManager_->Initialize();

	shockWaveManager_->SetDistortion(0.02f);
	shockWaveManager_->SetThickness(0.05f);
}

void ShockEffectSystem::Update()
{
	shockWaveManager_->SetCenter(screenPositionRatio_);
	if (effectTimer_.IsActive()) {
		shockWaveManager_->Update();
	}

	this->effectTimer_.Update();

}

void ShockEffectSystem::SetScreenPosition(const BaseCamera& camera)
{
	screenPosition_ = MathUtility::WorldToScreen(position_ - cameraAddPosition_, &const_cast<BaseCamera&>(camera));
	screenPositionRatio_ = MathUtility::ScreenPositionRatio(position_ - cameraAddPosition_, &const_cast<BaseCamera&>(camera));
}
void ShockEffectSystem::ImGuiDraw()
{
	ImGui::DragFloat2("ScreenPos", &screenPosition_.x);
	ImGui::DragFloat2("ScreenRatio", &screenPositionRatio_.x);
	shockWaveManager_->ImGuiDraw();
}

void ShockEffectSystem::StartShockWave(float frame)
{
	effectTimer_.Start(frame);
	shockWaveManager_->SetRadius(0.0f);
	position_ = weapon_->worldtransform_.GetWorldPosition();
}
