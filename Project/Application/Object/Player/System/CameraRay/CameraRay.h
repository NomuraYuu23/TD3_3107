#pragma once
#include "../../../IObject.h"
#include "../../../Engine/Collider2D/Segment2D.h"

class Player;

class CameraRay
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="player"></param>
	void Initialize(Player* player);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 
	/// </summary>
	/// <param name="target"></param>
	void OnCollision(ColliderParentObject2D target);

	void ImGuiDraw();
public:
	Segment2D GetSegment() { return directRay_; }
	Vector2 GetColliderPosition() { return { 0,0 }; }
	Vector2 GetColliderSize() { return {0,0}; }

	Segment2D directRay_;
private:
	// ポインタ
	Player* player_ = nullptr;


	// レイの方向
	Vector2 direct_ = {};

	float minPositionY_ = 0;

};