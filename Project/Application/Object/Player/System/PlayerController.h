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
	void InputUpdate();

private:
	// インプットクラス
	Input* input_ = nullptr;
	// プレイヤーのポインタ
	Player* player_ = nullptr;

};

