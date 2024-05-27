#pragma once
#include "../../IObject.h"
#include "../../../Engine/Collider2D/Segment2D.h"

class Enemy;

class DetectionRay
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="player"></param>
	void Initialize(Enemy* enemy);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 衝突判定
	/// </summary>
	/// <param name="target"></param>
	void OnCollision(ColliderParentObject2D target);

	void SetDirect(const Vector2& to, const Vector2& from) { direct_ = Vector2::Subtract(from, to); }

public:
	Segment2D GetSegment() { return directRay_; }
	Vector2 GetColliderPosition() { return { 0,0 }; }
	Vector2 GetColliderSize() { return { 0,0 }; }

	Segment2D directRay_;
private:
	// 敵
	Enemy* enemy_ = nullptr;

	// レイの方向
	Vector2 direct_ = {};

	float minPositionY_ = 0;


};