#pragma once
#include "../../Player.h"
#include "../../../Engine/Animation/Animation.h"

/// <summary>
/// プレイヤーアニメーション管理クラス
/// </summary>
class PlayerAnimation : public IObject
{
public: // メンバ関数

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="model">使用するモデル</param>
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
};

