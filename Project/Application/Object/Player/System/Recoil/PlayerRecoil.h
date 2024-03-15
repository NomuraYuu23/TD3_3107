#pragma once
#include "../../../GameUtility/TimerLib.h"
#include "../../../Engine/Math/Vector3.h"

class Player;

class PlayerRecoil
{
public:
	PlayerRecoil() {};
	~PlayerRecoil() = default;

public: // メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="player"></param>
	void Initialize(Player* player);
	/// <summary>
	/// 生成
	/// </summary>
	/// <param name="direction"></param>
	void CreateRecoil(const Vector3& direction);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

public: // アクセッサ
	/// <summary>
	/// 反動中かどうか
	/// </summary>
	/// <returns></returns>
	bool IsActive() { return timer_.IsActive(); }

private:
	// 親ポインタ
	Player* player_ = nullptr;
	// タイマー
	TimerLib timer_;
	// 反動の時間（フレーム
	float recoilFrame_;

private:
	// 加算速さ
	Vector3 velocity_ = {};
	// 反動の大きさ
	Vector3 recoilValue_ = {};

};

