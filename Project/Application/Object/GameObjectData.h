#pragma once
#include "../../Engine/GlobalVariables/GlobalVariables.h"

class GameObjectData
{
private:
	struct PlayerData {
		float normalJumpPower_;
		float highJumpPower_;
		float gravity_;
	};

	struct CommonData {
		float gravity_;
	};

	struct WeaponData {
		float gravity_;

	};

public:
	/// <summary>
	/// シングルトン
	/// </summary>
	/// <returns></returns>
	static GameObjectData* GetInstance() {
		static GameObjectData instance;
		return &instance;
	}

	void Initialize();

	void ApplyGlobalVariables();

private:
	// 
	GlobalVariables* globalVariables_ = nullptr;
	// 全体の情報
	CommonData common_;
	// プレイヤーの情報
	PlayerData player_;
	// 武器の情報
	WeaponData weapon_;
};

