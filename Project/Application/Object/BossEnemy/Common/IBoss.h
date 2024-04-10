#pragma once
#include "../../../../Engine/3D/Model.h"
#include "../../../../Engine/Collider2D/ColliderShape2D.h"
#include "../../GameUtility/CommonConfig.h"
#include "State/IBossState.h"

class IBoss
{
public:
	//virtual IBoss() {};
	virtual ~IBoss() = default;

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

	// マテリアル関係
	std::unique_ptr<Material> material_ = nullptr;
	//int32_t enableLighting_;
	//float shininess_;
	// 速さベクトル
	Vector3 velocity_ = {};

	// ステート
	std::unique_ptr<IBossState> state_;

protected:
	/// <summary>
	/// ステート変更関数
	/// </summary>
	/// <param name="newState"></param>
	void ChangeState(std::unique_ptr<IBossState> newState) {
		newState->PreInitialize(this);
		newState->Initialize();
		state_ = std::move(newState);
	}

};
