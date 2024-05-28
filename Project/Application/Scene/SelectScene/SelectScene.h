#pragma once
#include "../../../Engine/Scene/IScene/IScene.h"
#include "../../SelectSceneObject/SelectSystem.h"
#include "../../Skydome/Skydome.h"

class SelectScene : public IScene{

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() override;

private: // メンバ関数

	/// <summary>
	/// モデルクリエイト
	/// </summary>
	void ModelCreate() override;

	/// <summary>
	/// テクスチャロード
	/// </summary>
	void TextureLoad() override;

private: // 変数

	// テクスチャハンドル
	std::array<uint32_t, StageNumberManager::kStageMax> stagePhotTextureHandles_;
	std::array<uint32_t, StageSelectUI::SpriteIndex::kSpriteIndexOfCount> stageUITextureHandles_;

	// セレクトシステム
	std::unique_ptr<SelectSystem> selectSystem_;

	// シーン遷移が始まったか
	bool hasTheSceneTransitionStarted_;

	// スカイドーム
	std::unique_ptr<Skydome> skydome_;
	std::unique_ptr<Model> skydomeModel_;

	// スカイボックス
	uint32_t skyboxTextureHandle_ = 0;

};

