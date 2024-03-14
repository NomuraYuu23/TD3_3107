#pragma once
#include "../IObject.h"

/// <summary>
/// 地形用ブロック
/// </summary>
class Terrain : public IObject
{
public:
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
	void Draw(BaseCamera camera) override;
	/// <summary>
	/// ImGui
	/// </summary>
	void ImGuiDraw() override;

	/// <summary>
	/// 衝突のコールバック
	/// </summary>
	/// <param name="target"></param>
	/// <param name="tag"></param>
	void OnCollision(ColliderParentObject2D target) override;

	Vector2 GetColliderPosition() override { return boxCollider_.position_; }
	Vector2 GetColliderSize() override { return boxCollider_.scale_; }

private:
	// シリアルナンバー
	uint32_t serialNum_ = 0;

	static uint32_t sSerialNumber_;

	

};

