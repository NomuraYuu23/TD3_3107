#pragma once
#include "../../IObject.h"
#include "../../../Engine/3D/Model.h"
#include "../../../Engine/Collider2D/Box.h"

class CheckPointManager;

class CheckPointObject : public IObject
{
public:
	static uint32_t sSerialNumber;

	struct CheckPointData
	{
		Vector3 position;
		uint32_t checkPointNumber;
	};

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

private: // チェックポイント用変数
	uint32_t checkNum_ = 0u;
	CheckPointManager* checkPointManager_ = nullptr;
};
