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

	// スカイドーム
	std::unique_ptr<Skydome> skydome_;
	std::unique_ptr<Model> skydomeModel_;
	bool isDrawSkydome_ = true;

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

};

