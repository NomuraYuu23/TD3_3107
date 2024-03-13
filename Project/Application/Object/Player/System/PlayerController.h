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

	/// <summary>
	/// コントローラー操作処理
	/// </summary>
	void ControllerProcess();
	/// <summary>
	/// キーボード操作処理
	/// </summary>
	void KeyBoardProcess();

private:
	// インプットクラス
	Input* input_ = nullptr;
	// プレイヤーのポインタ
	Player* player_ = nullptr;

};

