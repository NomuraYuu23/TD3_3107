#pragma once
#include "../../../GameUtility/TimerLib.h"
#include "../../../Engine/Math/Vector3.h"
#include "../../../Engine/Math/Vector2.h"

class Player;

class AssistDash 
{
public:
	AssistDash() {};
	~AssistDash() = default;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="player"></param>
	void Initialize(Player* player) {
		player_ = player;
	}

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 受付開始関数
	/// </summary>
	/// <param name="acceptFrame"></param>
	void StartAccept(float acceptFrame);

	bool IsFallslowActive() { return fallSlowTimer_.IsActive(); }

	void SlowCancel() { fallSlowTimer_.End(); }

private:
	/// <summary>
	/// 加速処理
	/// </summary>
	void Dash();

private:
	Player* player_ = nullptr;

	// 入力受付時間
	TimerLib acceptTimer_;

	// ダッシュ後の余韻時間（落下速度などが落ちる時間）
	TimerLib fallSlowTimer_;

	// 入力方向
	Vector2 inputDirect_ = {};

};
