#pragma once
#include <numbers>
#include "../IObject.h"

/// <summary>
/// 背景クラス
/// </summary>
class BackGround : public IObject
{
public: // メンバ関数

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
	/// 衝突コールバック
	/// </summary>
	/// <param name="target"></param>
	/// <param name="tag"></param>
	void OnCollision(ColliderParentObject2D target) override;

public: // アクセッサ
	/// <summary>
	/// コライダーの取得
	/// </summary>
	/// <returns></returns>
	Box GetBoxCollider() override;

	Vector2 GetColliderPosition() override;
	Vector2 GetColliderSize() override;
};

