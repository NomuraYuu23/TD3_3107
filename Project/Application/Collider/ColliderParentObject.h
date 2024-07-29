#pragma once
#include <variant>
#include "../../Engine/Collision/CollisionData.h"

class Null;
class Player;
class Block;
class Screw;
class Magnet;
class UFO;
class UFOAttract;
class Energy;
class Planet;
using ColliderParentObject = std::variant<Null*, Player*,Block*,Magnet*,Screw*,UFO*,UFOAttract*,Energy*,Planet*>;

// 親がないもの用
class Null {

public:

	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="colliderPartner"></param>
	/// <param name="collisionData"></param>
	void OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData) {}

};

