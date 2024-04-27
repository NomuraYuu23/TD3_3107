#pragma once
#include "../../../Engine/Input/Input.h"
#include "../../../Engine/3D/Model.h"

class Player;

class PlayerController
{
public:
	PlayerController() {};
	~PlayerController() = default;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="player"></param>
	void Initialize(Player* player);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	void ImGuiDraw();

private: // 外付け機器の処理（大枠
	/// <summary>
	/// コントローラー操作処理
	/// </summary>
	void ControllerProcess();
	/// <summary>
	/// キーボード操作処理
	/// </summary>
	void KeyBoardProcess();

private: // ステートごとの処理
	/// <summary>
	/// 空中での操作
	/// </summary>
	void AerialMoveProcess();
	/// <summary>
	/// 地上での操作
	/// </summary>
	void GroundMoveProcess();

	/// <summary>
	/// 槍に乗った待機状態の操作
	/// </summary>
	void WaitKeyProcess();


private: // 操作内容関数
	/// <summary>
	/// 投げの処理
	/// </summary>
	void ThrownProcess();

private:
	// インプットクラス
	Input* input_ = nullptr;
	// プレイヤーのポインタ
	Player* player_ = nullptr;
private:
	// 地上の速度
	float groundSpeed_ = 0;
	// 空中の速度
	float aerialSpeed_ = 0;
};

