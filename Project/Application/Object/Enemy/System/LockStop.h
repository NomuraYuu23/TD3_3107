#pragma once
#include "../../GameUtility/TimerLib.h"

class Enemy;

class LockStop
{
public:
	void Initialize(Enemy* enemy);

	void Update();
private:
	Enemy* enemy_ = nullptr;

	// ストップフラグ修正
	TimerLib acceptTimer_;

};
