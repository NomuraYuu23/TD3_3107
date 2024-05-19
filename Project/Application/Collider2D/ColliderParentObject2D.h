#pragma once
#include <variant>

class Sample;
class SampleObject;

// プレイヤー
class Player;
class PlayerFootCollider;
class CameraRay;
class Weapon;

// 地形
class Terrain;

// 敵
class Enemy;
// 仮の名前
class PrevSmallBoss;
class GoalObject;

using ColliderParentObject2D = std::variant<Terrain*, Player*, PlayerFootCollider*, CameraRay*, Weapon*, Enemy*, PrevSmallBoss*, GoalObject*>;
