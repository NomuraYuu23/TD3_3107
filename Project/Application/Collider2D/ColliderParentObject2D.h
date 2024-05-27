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
class DetectionRay;

// 仮の名前
class PrevSmallBoss;
// ゴールと中継地点オブジェ
class GoalObject;
class CheckPointObject;

using ColliderParentObject2D = std::variant<Terrain*, Player*, PlayerFootCollider*, CameraRay*, Weapon*, Enemy*, PrevSmallBoss*, GoalObject*, CheckPointObject*, DetectionRay*>;
