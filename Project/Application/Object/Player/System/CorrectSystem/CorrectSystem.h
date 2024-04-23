#pragma once
#include "../../../Engine/Collider2D/Circle.h"

class Player;

class CorrectSystem
{
public:
	void Initialize(Player* player);

	Player* player_;

	Circle circleCollider_;

};
