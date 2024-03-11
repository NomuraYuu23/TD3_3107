#pragma once
#include "IActionState.h"
#include "../../GameUtility/TimerLib.h"

class ActionWaitState : public IActionState
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

	TimerLib changeTimer_;

};

