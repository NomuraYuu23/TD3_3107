#pragma once
#include "../../IObject.h"
#include "../../../Engine/3D/Model.h"
#include "../../../Engine/Collider2D/Box.h"

#include "../../../AllSceneObject/StageNumberManager.h"

class GoalObject : public IObject
{

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

private:
	void SystemInitialize();

private: //メンバ関数

	/// <summary>
	/// 調整項目適用関数
	/// </summary>
	void ApplyGlobalVariables();

public: // 関数

	/// <summary>
	///	位置設定
	/// </summary>
	void SetPosition();

public: // アクセッサ
	// コライダー関係
	Vector2 GetColliderPosition() override { return circleCollider_.position_; }
	Vector2 GetColliderSize() override { return boxCollider_.scale_; }
	Box GetBoxCollider() override { return boxCollider_; }

	// ゴールフラグ取得
	bool IsGoal() { return isGoal_; }
	void SetIsGoal(bool isGoal) { isGoal_ = isGoal; }

private:
	// ゴールフラグ
	bool isGoal_ = false;

public: // 変数

	// 各ステージのゴール
	std::array<Vector3, StageNumberManager::kStageMax> goalPositions_;

};