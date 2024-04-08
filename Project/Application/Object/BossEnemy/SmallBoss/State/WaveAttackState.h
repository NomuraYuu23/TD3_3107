#pragma once
#include "../../Common/State/IBossState.h"
#include "../../../GameUtility/TimerLib.h"

class WaveAttackState : public IBossState
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

private:
	/// <summary>
	/// 仮の作成関数
	/// </summary>
	void CreateWave(const Vector3& direct, const Vector3& offset);

	TimerLib generateTimer_;

	uint32_t generateCount_ = 0;

};