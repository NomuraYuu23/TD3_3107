#pragma once
#include "../../../Engine/Scene/IScene/IScene.h"

#include "../../AudioManager/TitleAudioManager.h"

#include "../../Skydome/Skydome.h"

class TitleScene : public IScene
{

public: // メンバ関数

	~TitleScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	/// <summary>
	/// imgui描画処理
	/// </summary>
	void ImguiDraw();

private: // メンバ関数

	/// <summary>
	/// モデルクリエイト
	/// </summary>
	void ModelCreate() override;

	/// <summary>
	/// テクスチャロード
	/// </summary>
	void TextureLoad() override;

private:

	/// <summary>
	/// 音量
	/// </summary>
	void LowerVolumeBGM();

private: // メンバ変数

	// オーディオマネージャー
	std::unique_ptr<TitleAudioManager> audioManager_;

	bool isDecreasingVolume = true;

	// パーティクルマネージャー
	ParticleManager* particleManager_ = nullptr;
	std::unique_ptr<Model> particleUvcheckerModel_ = nullptr;
	std::unique_ptr<Model> particleCircleModel_ = nullptr;
	std::unique_ptr<Model> particleLeafModel_ = nullptr;
	std::unique_ptr<Model> particleSpearLeafModel_ = nullptr;
	std::unique_ptr<Model> particleSmokeModel_ = nullptr;

	// スカイドーム
	std::unique_ptr<Skydome> skydome_;
	std::unique_ptr<Model> skydomeModel_;
	bool isDrawSkydome_ = true;
	uint32_t skyDomeTexHandle_ = 0u;

	// 環境パーティクル生成用座標
	std::unique_ptr<EulerTransform> emitTransform_;
	// 環境パーティクル用設定構造体
	EmitterDesc fallingLeafDesc_;

	// タイトルロゴスプライト
	std::unique_ptr<Sprite> titleSprite_;
	// ボタンスプライト
	std::unique_ptr<Sprite> buttonSprite_;

	// タイトルロゴテクスチャ
	uint32_t logoTexHandle_ = 0u;
	// ボタンテクスチャ
	uint32_t buttonTexHandle_ = 0u;

	// スカイボックス
	uint32_t skyboxTextureHandle_ = 0;

	// ボタンスプライト用演出変数
	float currentButtonStagingTime_ = 0.0f; // 現在演出時間
	float ButtonStagingTime_ = 1.5f;		// 演出時間
	bool isReturn_ = false;					// フラグ
	Vector2 prevPos_;						// 事前座標
	Vector2 postPos_;						// 処理後座標

};

