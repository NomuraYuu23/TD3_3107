#pragma once
#include <numbers>
#include "../IObject.h"

// クラスの前方宣言
class Player;

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
	
	/// <summary>
	/// チュートリアル用平面モデルセッター
	/// </summary>
	/// <param name="texHandleManager">テクスチャハンドルマネージャー</param>
	/// <param name="spearJump">槍ジャンプ</param>
	/// <param name="enemy">敵</param>
	void SetTutorialPlaneModel(ITextureHandleManager* texHandleManager, Model* spearJump, Model* enemy);

private: // メンバ変数

	// DirectX汎用クラスのインスタンス
	DirectXCommon* dxCommon_ = nullptr;
	// テクスチャハンドルのマネージャー
	ITextureHandleManager* texHandleManager_ = nullptr;

	// 二個目の背景オブジェクトのトランスフォーム
	WorldTransform transform2_;

	// チュートリアル平面モデル
	Model* speartutorialPlaneModel_ = nullptr;
	Model* enemytutorialPlaneModel_ = nullptr;

	// チュートリアル平面用マテリアル
	std::unique_ptr<Material> tutorialPlaneMaterial_;

	// テクスチャハンドル配列
	std::map<int32_t, int32_t> spearTutorialTexHandles_;
	std::map<int32_t, int32_t> enemyTutorialTexHandles_;

	float spearCurrentTime_ = 0.0f;
	float enemyCurrentTime_ = 0.0f;
	float spearStagingTime_ = 0.0f;
	float enemyStagingTime_ = 0.0f;

	float updateTime_ = 1.0f / 60.0f;

	// チュートリアル用ワールドトランスフォーム
	WorldTransform spearTutorialTransform_;
	WorldTransform enemyTutorialTransform_;

};

