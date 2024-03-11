#pragma once
#include "../../../Engine/Input/Input.h"

class Player;

class PlayerController
{
public:
	PlayerController() {};
	~PlayerController() = default;

public:
	void Initialize(Player* player);
	void Update();

	void ControllerProcess();
	void KeyBoardProcess();

private:
	// インプットクラス
	Input* input_ = nullptr;
	// プレイヤーのポインタ
	Player* player_ = nullptr;

};

