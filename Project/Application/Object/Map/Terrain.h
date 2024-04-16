#pragma once
#include "../IObject.h"
#include "../../../Engine/3D/OneOfManyObjects.h"

/// <summary>
/// 地形用ブロック
/// </summary>
class Terrain : public OneOfManyObjects
{
public:
	//種類
	enum class BlockType : uint32_t
	{
		kNone,	// 当たり判定に入れない
		kTerrain,	// 地形ブロック（カメラに作用するブロック
		kObstacle,	// 障害物ブロック（カメラに作用しないブロック
		kMaxSize,
	};

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// ImGui
	/// </summary>
	void ImGuiDraw();

	/// <summary>
	/// 衝突のコールバック
	/// </summary>
	/// <param name="target"></param>
	/// <param name="tag"></param>
	void OnCollision(ColliderParentObject2D target);

	Vector2 GetColliderPosition() { return boxCollider_.position_; }
	Vector2 GetColliderSize() { return boxCollider_.scale_; }
	Box GetBoxCollider() { return boxCollider_; }

	void BoxColliderUpdate() {
		boxCollider_.Update(position2D_, scale2D_.x, scale2D_.y, 0.0f);
	}

private:
	// シリアルナンバー
	uint32_t serialNum_ = 0;

	static uint32_t sSerialNumber_;

public:

	// コライダー用の座標・スケール
	Vector2 position2D_ = {};
	Vector2 scale2D_ = {};

	// コライダー
	Box boxCollider_;

	// 地面か障害物か
	BlockType typeNumber_ = BlockType::kTerrain;

};

