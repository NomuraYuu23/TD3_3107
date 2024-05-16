#pragma once
#include "../../GameUtility/TimerLib.h"

class Weapon;

class AutoComeBack
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="weapon"></param>
	void Initialize(Weapon* weapon);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 開始関数
	/// </summary>
	/// <param name="endFrame"></param>
	void Start(float endFrame);

	/// <summary>
	/// 終了フラグ
	/// </summary>
	/// <returns></returns>
	bool IsEnd() { return endTimer_.IsEnd(); }

private:
	// 武器
	Weapon* weapon_ = nullptr;
	// タイマー
	TimerLib endTimer_;

};