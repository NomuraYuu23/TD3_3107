#pragma once
#include "../../../Engine/Collider2D/Circle.h"
#include "../../../Enemy/EnemyManager.h"

class Player;

class CorrectSystem
{
private:
	struct AssistValue {
		// 角度
		float rotationWidth_;
		// 領域範囲
		float InitLength_;
		// エイム補正の幅
		float assistWidth_;
	};
public:
	enum AssistLevel {
		kNone,		// なし
		kNormal,	// 普通
		kHard,		// 大きい
	};

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="player">プレイヤー</param>
	void Initialize(Player* player);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="enemyManager">エネミーマネージャー</param>
	void Update(EnemyManager* enemyManager);

	void ImGuiDraw();

	bool IsLockOn() { return isLockOn_; }

	OneOfManyObjects* GetTarget() {
		if (targetPointer_) {
			return targetPointer_;
		}
		return nullptr;
	}

	void TargetStop();

	/// <summary>
	/// システムのフラグセッター
	/// </summary>
	/// <param name="isSystem"></param>
	void SetIsSystem(bool isSystem) { isSystem_ = isSystem; }
public:
	// アシストのレベル
	int32_t assistLevel_ = kNormal;

private:
	// システムのオンオフ
	bool isSystem_ = false;

	// アシストの値
	AssistValue assistValue_ = {};

	// アシスト値の設定関数（レベル変えてから呼び出せば変わる
	void SetAssistValue();

private:

	void NearLockOn(EnemyManager* enemyManager);

	// プレイヤーポインタ
	Player* player_;

	Vector3 NearEnemyLockOn(EnemyManager* enemyManager);

	Vector3 StickAimAssist(EnemyManager* enemyManager, const Vector3& stickDirect);

	Vector3 LeftStickAimAssist(EnemyManager* enemyManager, const Vector3& stickDirect);

	Vector2 prevLeftStick_ = {};

	bool isInNearArea_ = false;
	bool isLockOn_ = false;


	Vector3 targetDirect_ = {};

	OneOfManyObjects* targetPointer_ = nullptr;
};
