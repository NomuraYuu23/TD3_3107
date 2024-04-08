#pragma once
#include "../../../IObject.h"

class IAttackObject
{
public:
	// シリアルナンバー
	uint32_t serialNum_ = 0;

	static uint32_t sSerialNumber_;

public:
	virtual ~IAttackObject() = default;

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
	virtual void Draw(const BaseCamera& camera) = 0;
	/// <summary>
	/// ImGui
	/// </summary>
	virtual void ImGuiDraw() = 0;

	/// <summary>
	/// 衝突のコールバック
	/// </summary>
	/// <param name="target"></param>
	/// <param name="tag"></param>
	virtual void OnCollision(ColliderParentObject2D target) = 0;

public: // アクセッサ
	/// <summary>
	/// コライダーの取得
	/// </summary>
	/// <returns></returns>
	virtual Box GetBoxCollider() = 0;
	virtual Vector2 GetColliderPosition() = 0;
	virtual Vector2 GetColliderSize() = 0;
protected:
	// ボックスコライダーの更新
	void BoxColliderUpdate() {
		boxCollider_.Update(position2D_, scale2D_.x, scale2D_.y, 0.0f);
	}

public: // 
	// ワールドトランスフォーム
	WorldTransform worldtransform_;

	// コライダー用の座標・スケール
	Vector2 position2D_ = {};
	Vector2 scale2D_ = {};

	// コライダー
	Box boxCollider_;

protected: // 
	// モデル
	Model* model_ = nullptr;
	// マテリアル関係
	std::unique_ptr<Material> material_ = nullptr;

};