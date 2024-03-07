#pragma once
#include <variant>

class Sample;
class Player;
class Weapon;
class Enemy;
class SampleObject;
class IObject;

using ColliderParentObject2D = std::variant<Player*, IObject*, Weapon*>;
