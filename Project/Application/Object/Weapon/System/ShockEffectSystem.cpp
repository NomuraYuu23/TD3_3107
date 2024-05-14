#include "ShockEffectSystem.h"

void ShockEffectSystem::Initialize(Weapon* weapon)
{
	weapon_ = weapon;
	// 衝撃波マネージャの初期化
	shockWaveManager_ = std::make_unique<ShockWaveManager>();
	shockWaveManager_->Initialize();
}

void ShockEffectSystem::Update(const BaseCamera& camera)
{
	shockWaveManager_->Update();
}
