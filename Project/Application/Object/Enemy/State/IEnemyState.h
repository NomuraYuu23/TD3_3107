#pragma once
#include <stdint.h>
#include <array>

class Enemy;

class IEnemyState
{
public:
	/// <summary>
	/// 行動パターン
	/// </summary>
	enum class ActionMode : uint32_t
	{
		kPatrolMode,	// 巡回
		kChaseMode,		// 追跡

		kMaxSize, // 最大値
	};

public:
	/// <summary>
	/// 前初期化
	/// 回転する集団は関係ない
	/// 0 : 巡回
	/// 1 : 追跡
	/// </summary>
	void PreInitialize(Enemy* enemy, ActionMode pattern);


	void PreInitialize(Enemy* enemy);

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize() = 0;
	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();

protected: // それぞれの行動関数
	// 近距離
	virtual void PatrolInitialize() {};
	virtual void PatrolUpdate() {};

	// 遠距離
	virtual void ChaseInitialize() {};
	virtual void ChaseUpdate() {};

protected:
	// 関数ポインタの構造体
	struct MovementFunc {
		void(IEnemyState::* initFunc_)() = nullptr;
		void(IEnemyState::* updateFunc_)() = nullptr;
	};

protected:
	// 親のエネミーポインタ
	Enemy* enemy_ = nullptr;
	// 落下フラグ
	bool isFall_ = false;
	// 行動のパターン
	uint32_t actionPattern_;

private:

};
