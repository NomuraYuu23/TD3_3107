#pragma once
#include "../../Engine/GlobalVariables/GlobalVariables.h"

class GameObjectData
{
private:
	struct PlayerData {
		// 通常のジャンプ量
		float normalJumpPower_;
		// 槍ジャンプ
		float highJumpPower_;
		// 重力
		float gravity_;

		// 地上の移動量
		float moveValue_;
		// 空中の加速度
		float aerialAcceleration_;

	};

	struct CommonData {
		// 重力
		float gravity_;
	};

	struct WeaponData {
		// 重力
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

