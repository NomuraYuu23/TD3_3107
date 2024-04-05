#pragma once
#include <variant>

class Sample;
class SampleObject;

// プレイヤー
class Player;
class PlayerFootCollider;
class Weapon;

// 地形
class Terrain;

// 敵
class Enemy;
// 仮の名前
class PrevSmallBoss;

using ColliderParentObject2D = std::variant<Terrain*, Player*, PlayerFootCollider*, Weapon*, Enemy*, PrevSmallBoss*>;
