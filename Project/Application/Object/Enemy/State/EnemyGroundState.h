#pragma once
#include "IEnemyState.h"

class EnemyGroundState : public IEnemyState
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

};