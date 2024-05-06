#pragma once
#include "../../Engine/GlobalVariables/GlobalVariables.h"
#include "../../Engine/Math/Vector2.h"
#include "../../Engine/Math/Vector3.h"

class GameObjectData
{
private: // データ用構造体クラス
	// 反動の情報
	struct RecoilData {
		// 反動の値
		float ratio_;
		// 反動の時間
		float time_;
		// Lerpの値
		float lerpRatio_;
	};

	/// <summary>
	/// ジャンプの情報
	/// </summary>
	struct JumpData {
		// 通常のジャンプ量
		float normalJumpPower_;
		// 槍ジャンプ
		float highJumpPower_;
		// 重力
		float gravity_;

		// 槍じゃんの水平方向の力
		float horizontalPower_;

	};

	/// <summary>
	/// 移動の情報
	/// </summary>
	struct MoveData {
		// 地上の移動量
		float moveValue_;
		// 空中の加速度
		float aerialAcceleration_;
		// 空中の逆方向慣性のレシオ
		float invAerialRatio_;
	};

	/// <summary>
	/// 体力関係の情報
	/// </summary>
	struct HealthData {
		// 最大体力
		int32_t hp_;
		// 無敵時間
		float invTimer_;
	};

	/// <summary>
	/// 補正の情報
	/// </summary>
	struct CorrectData {
		// 角度
		float rotationWidth_;
		// 領域範囲
		float InitLength_;
	};

	// プレイヤーの情報
	struct PlayerData {
		// ジャンプデータ
		JumpData jumpData_;
		// 移動データ
		MoveData moveData_;
		// 反動情報
		RecoilData recoil;

		// 画面外から戻ってくる際の画面外の距離
		float deadLength_;
	
		// 体力データ
		HealthData hpData_;

		CorrectData correctData_;
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
		// ローカル座標
		Vector3 localPosition_;
		// 衝突時の避ける内積の値
		float collisionDot_;

		// 補正の幅
		float assistWidth_;

		float kickBackCooltime_;
	};

	struct BossData {
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

