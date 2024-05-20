#pragma once
#include "IEnemyState.h"
#include <array>

class GroupEnemyState : public IEnemyState
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
	void PatrolInitialize() override;
	void PatrolUpdate() override;

	// 遠距離
	void ChaseInitialize() override;
	void ChaseUpdate() override;
private:

	void MoveUpdate();

private:
	// 行動の関数ポインタを持たせた配列
	inline static const std::array<MovementFunc, static_cast<uint32_t>(ActionMode::kMaxSize)> actionFuncs_{
		MovementFunc{&IEnemyState::PatrolInitialize,&IEnemyState::PatrolUpdate,},
		MovementFunc{&IEnemyState::ChaseInitialize,&IEnemyState::ChaseUpdate,}
	};

};