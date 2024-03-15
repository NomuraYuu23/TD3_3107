#pragma once
#include "../IObject.h"
#include "../../../Engine/Animation/Animation.h"

/// <summary>
/// 槍クラス
/// </summary>
class Spear :
    public IObject
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

private: // メンバ変数

	// アニメーション
	Animation anim_;

};

