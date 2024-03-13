#pragma once
#include "../../Engine/3D/Model.h"
#include "../../Engine/Collider2D/ColliderShape2D.h"

class IObject
{
public:
	IObject() {};
	~IObject() = default;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	virtual void Initialize(Model* model);
	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	virtual void Draw(BaseCamera camera) = 0;
	/// <summary>
	/// ImGui
	/// </summary>
	virtual void ImGuiDraw() = 0;

	/// <summary>
	/// 衝突のコールバック
	/// </summary>
	/// <param name="target"></param>
	/// <param name="tag"></param>
	virtual void OnCollision(ColliderParentObject2D target, const Vector2& targetPosition) = 0;

public: // アクセッサ
	/// <summary>
	/// コライダーの取得
	/// </summary>
	/// <returns></returns>
	Box GetCollider() {
		return boxCollider_;
	}

	virtual Vector2 GetCollierPosition() = 0;
protected:
	void BoxColliderUpdate() {
		boxCollider_.Update(position2D_, scale2D_.x, scale2D_.y, 0.0f);
	}

	void CircleColliderUpdate() {
		circleCollider_.Update(position2D_, scale2D_.x);
	}

public:
	// モデル
	Model* model_ = nullptr;
	// ワールドトランスフォーム
	WorldTransform worldtransform_;

	// コライダー用の座標・スケール
	Vector2 position2D_ = {};
	Vector2 scale2D_ = {};

	// コライダー
	Box boxCollider_;
	Circle circleCollider_;

	// マテリアル関係
	std::unique_ptr<Material> material_ = nullptr;
	int32_t enableLighting_;
	float shininess_;

};

