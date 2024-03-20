#pragma once
#include "../../Engine/GlobalVariables/GlobalVariables.h"

class GameObjectData
{
private:
	struct PlayerData {
		float normalJumpPower_;
		float highJumpPower_;
	};

	struct CommonData {
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
	// プレイヤーの情報
	PlayerData player_;
	// 
	CommonData common_;
};

