#include "WeaponGuardEffect.h"
#include "../../Player.h"

void WeaponGuardEffect::Initialize(Player* player)
{

	player_ = player;
	// 衝撃波マネージャの初期化
	shockWaveManager_ = std::make_unique<ShockWaveManager>();
	shockWaveManager_->Initialize();

	shockWaveManager_->SetDistortion(0.02f);
	shockWaveManager_->SetThickness(0.05f);

}

void WeaponGuardEffect::Update()
{

	shockWaveManager_->SetCenter(screenPositionRatio_);
	if (effectTimer_.IsActive()) {
		shockWaveManager_->Update();
	}

	this->effectTimer_.Update();

}

void WeaponGuardEffect::SetScreenPosition(const BaseCamera& camera)
{

	screenPositionRatio_ = MathUtility::ScreenPositionRatio(position_ - cameraAddPosition_, &const_cast<BaseCamera&>(camera));

}

void WeaponGuardEffect::StartShockWave(float frame) {
	effectTimer_.Start(frame);
	shockWaveManager_->SetRadius(0.0f);
	position_ = player_->worldtransform_.GetWorldPosition();
}

void WeaponGuardEffect::ImGuiDraw()
{
}
