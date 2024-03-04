#pragma once
#include "../../../Engine/3D/Model.h"
#include "../../../Engine/Collider2D/Box.h"

class SampleObject : public Box
{

public:

	~SampleObject();

	void Initialize(Model* model);

	void Update();

	void Draw(BaseCamera camera);

	void ImGuiDraw();

private:

	/// <summary>
	/// 外部変数登録
	/// </summary>
	virtual void RegisteringGlobalVariables();

	/// <summary>
	/// 外部変数適用
	/// </summary>
	virtual void ApplyGlobalVariables();

private:

	// モデル
	Model* model_ = nullptr;
	// マテリアル
	std::unique_ptr<Material> material_ = nullptr;
	// ワールドトランスフォーム
	WorldTransform worldtransform_;

	// コライダー用の座標・スケール
	Vector2 position2D_ = {};
	Vector2 scale2D_ = {};

	// 
	int32_t enableLighting_;

	float shininess_;

};

