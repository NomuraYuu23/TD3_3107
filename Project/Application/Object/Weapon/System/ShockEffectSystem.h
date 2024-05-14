#pragma once
#include "../Weapon.h"
#include "../../../Engine/PostEffect/ShockWaveManager.h"

class ShockEffectSystem 
{
public:
	void Initialize(Weapon* weapon);
	void Update(const BaseCamera& camera);
	ShockWaveManager* GetShockWaveManager() { return shockWaveManager_.get(); }
	void SetScreenPosition(const Vector2& position) { screenPositionRatio_ = position; }
private:
	Weapon* weapon_ = nullptr;
	Vector2 screenPositionRatio_ = {};

	std::unique_ptr<ShockWaveManager> shockWaveManager_;
};
