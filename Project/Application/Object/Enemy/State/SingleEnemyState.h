#pragma once
#include "IEnemyState.h"

class SingleEnemyState : public IEnemyState
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;
private:
	// 近距離
	void MeleeInitialize() override;
	void MeleeUpdate() override;

	// 遠距離
	void RangedInitialize() override;
	void RangedUpdate() override;

private:
	// 行動の関数ポインタを持たせた配列
	inline static const std::array<MovementFunc, static_cast<uint32_t>(AttackPattern::kMaxSize)> actionFuncs_{
		MovementFunc{&IEnemyState::MeleeInitialize,&IEnemyState::MeleeUpdate,},
		MovementFunc{&IEnemyState::RangedInitialize,&IEnemyState::RangedUpdate,}
	};
};