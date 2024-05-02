#pragma once
#include "../../../GameUtility/TimerLib.h"

class FreeFallTimer
{
public:
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 開始
	/// </summary>
	/// <param name="endFrame"></param>
	void StartSetting(float endFrame) {
		timer_.Start(endFrame);
	}

	/// <summary>
	/// 強制終了関数
	/// </summary>
	void ForcedEnd() {
		timer_.End();
	}

	/// <summary>
	/// 終了フラグ
	/// </summary>
	/// <returns></returns>
	bool IsEnd() { return timer_.IsEnd(); }

	/// <summary>
	/// アクティブフラグ
	/// </summary>
	/// <returns></returns>
	bool IsActive() { return timer_.IsActive(); }

private:
	TimerLib timer_;

};