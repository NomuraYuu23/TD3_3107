#pragma once
#include <variant>

class Sample;
class Player;
class Weapon;
class Enemy;
class SampleObject;
class IObject;
class Terrain;
class PlayerFootCollider;
class Enemy;

using ColliderParentObject2D = std::variant<Player*, IObject*, Weapon*, Terrain*, PlayerFootCollider*, Enemy*>;
