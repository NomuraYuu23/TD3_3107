#pragma once
#include "../../IObject.h"
#include "../../../Engine/3D/Model.h"
#include "../../../Engine/Collider2D/Box.h"
#include "CheckPointData.h"

class CheckPointManager;

class CheckPointObject : public IObject
{
public:
	static uint32_t sSerialNumber;

public: // 継承
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	void Initialize(Model* model) override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	void Draw(const BaseCamera& camera) override;
	/// <summary>
	/// ImGui
	/// </summary>
	void ImGuiDraw() override;
	/// <summary>
	/// 衝突判定
	/// </summary>
	/// <param name="target"></param>
	/// <param name="tag"></param>
	void OnCollision(ColliderParentObject2D target) override;

public: // アクセッサ
	Vector2 GetColliderPosition() override { return circleCollider_.position_; }
	Vector2 GetColliderSize() override { return boxCollider_.scale_; }
	Box GetBoxCollider() override { return boxCollider_; }
	// 座標
	void SetPosition(const Vector3& position) { worldtransform_.transform_.translate = position; }

	void SetManager(CheckPointManager* checkPointManager) { checkPointManager_ = checkPointManager; }

	void Setting(const CheckPointData& data);

	/// <summary>
	/// 通過後モデルセッター
	/// </summary>
	/// <param name="model">通過後表示するモデル</param>
	void SetPassedModel(Model* model);

private: // 演出系

	// 通過後モデル
	Model* passedModel_ = nullptr;

	// 通過後モデル用マテリアル
	std::unique_ptr<Material> passedMaterial_;

	// 通過後モデル用トランスフォーム
	WorldTransform passedModelTransform_;

	// 
	std::unique_ptr<LocalMatrixManager> passedLocalMatrixManager_;

	// チェックポイント通過演出用時間
	float currentTime_ = 0.0f; // 現在時間
	float stagingTime_ = 2.0f; // 演出時間

	// 通過したか
	bool isPassed_ = false;

private: // チェックポイント用変数
	uint32_t checkNum_ = 0u;
	CheckPointManager* checkPointManager_ = nullptr;
};
