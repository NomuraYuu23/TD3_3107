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

	/// <summary>
	/// 強制終了関数
	/// </summary>
	void CancelRecoil();

public: // アクセッサ
	/// <summary>
	/// 反動中かどうか
	/// </summary>
	/// <returns></returns>
	bool IsActive() { return timer_.IsActive(); }

	/// <summary>
	/// 受け付ける処理
	/// </summary>
	void Accept() { isAccept_ = true; }

	/// <summary>
	/// 反動を受け付けたか
	/// </summary>
	/// <returns></returns>
	bool IsAccept() { return isAccept_; }

private: // 管理
	// 親ポインタ
	Player* player_ = nullptr;
	// タイマー
	TimerLib timer_;
	// 反動の時間（フレーム
	float recoilFrame_;

private: // 移動
	// 加算速さ
	Vector3 velocity_ = {};
	// 反動の大きさ
	Vector3 recoilValue_ = {};
	// ゲームスピードに応じた更新回数用の値
	float slowValue_ = 0;

private:
	// 反動を受け付けたか
	// true = Yes, false = No
	bool isAccept_;

};

