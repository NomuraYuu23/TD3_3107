#pragma once
#include "../IObject.h"

class Player;

class PlayerFootCollider
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	void Initialize(Model* model, Player* parent);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// ImGui
	/// </summary>
	void ImGuiDraw();
	/// <summary>
	/// 衝突コールバック
	/// </summary>
	/// <param name="target"></param>
	/// <param name="tag"></param>
	void OnCollision(ColliderParentObject2D target);
	void DebugDraw(BaseCamera camera);

	Vector2 GetColliderPosition() { return boxCollider_.position_; }
	Vector2 GetColliderSize() { return boxCollider_.scale_; }
	Box GetBoxCollider() { return boxCollider_; }
	// コライダー
	Box boxCollider_;

private:
	Player* player_ = nullptr;

private:
	// モデル
	Model* model_ = nullptr;
	// ワールドトランスフォーム
	WorldTransform worldtransform_;

	// コライダー用の座標・スケール
	Vector2 position2D_ = {};
	Vector2 scale2D_ = {};


	// マテリアル関係
	std::unique_ptr<Material> material_ = nullptr;
	int32_t enableLighting_;
	float shininess_;

};

