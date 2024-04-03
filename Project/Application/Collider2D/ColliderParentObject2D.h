#pragma once
#include <variant>

class Sample;
class SampleObject;

class Player;
class PlayerFootCollider;
class Weapon;
class Enemy;
class Terrain;

using ColliderParentObject2D = std::variant<Terrain*, Player*, PlayerFootCollider*, Weapon*, Enemy*>;
