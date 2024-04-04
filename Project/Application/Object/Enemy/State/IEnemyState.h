#pragma once
#include <stdint.h>
#include <array>

class Enemy;

class IEnemyState
{
public:
	/// <summary>
	/// 攻撃パターン
	/// </summary>
	enum class AttackPattern : uint32_t
	{
		kMelee,	 // 近距離
		kRanged, // 遠距離

		kMaxSize, // 最大値
	};

public:
	/// <summary>
	/// 前初期化
	/// 0 : 近接
	/// 1 : 遠距離
	/// </summary>
	void PreInitialize(Enemy* enemy, AttackPattern pattern);
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
	virtual void MeleeInitialize() = 0;
	virtual void MeleeUpdate() = 0;

	// 遠距離
	virtual void RangedUpdate() = 0;
	virtual void RangedInitialize() = 0;

protected:
	// 関数ポインタの構造体
	struct MovementFunc {
		void(IEnemyState::* initFunc_)() = nullptr;
		void(IEnemyState::* updateFunc_)() = nullptr;
	};

	//std::array<MovementFunc, static_cast<uint32_t>(AttackPattern::kMaxSize)> actionFuncs_{
	//	MovementFunc{&IEnemyState::MeleeInitialize,&IEnemyState::MeleeUpdate,},
	//	MovementFunc{&IEnemyState::RangedInitialize,&IEnemyState::RangedUpdate,}
	//};

protected:
	// 親のエネミーポインタ
	Enemy* enemy_ = nullptr;
	// 落下フラグ
	bool isFall_ = false;
	// 攻撃のパターン
	uint32_t attackPattern_;

private:

};
