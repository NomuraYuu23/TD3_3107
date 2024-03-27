#pragma once
#include "../../Engine/GlobalVariables/GlobalVariables.h"

class GameObjectData
{
private: // データ用構造体クラス
	// 反動の情報
	struct RecoilData {
		// 反動の値
		float ratio_;
		// 反動の時間
		float time_;
	};

	// プレイヤーの情報
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
		// 反動情報
		RecoilData recoil;
	};
	// 共通の情報
	struct CommonData {
		// スロー時の値
		float slowMotionFactor_;
		// 重力
		float gravity_;
	};

	// 武器の情報
	struct WeaponData {
		// 重力
		float gravity_;
		// 移動速度
		float speedRatio_;
		// 
		float scaleRate_;
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

