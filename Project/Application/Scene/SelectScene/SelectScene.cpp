#include "SelectScene.h"

void SelectScene::Initialize()
{

	IScene::Initialize();

	ModelCreate();
	TextureLoad();

	selectSystem_ = std::make_unique<SelectSystem>();
	selectSystem_->Initialize(stagePhotTextureHandles_, stageUITextureHandles_);

	hasTheSceneTransitionStarted_ = false;

}

void SelectScene::Update()
{

	// システム更新
	selectSystem_->Update();

	// シーン遷移が始まってたらリターン
	if (hasTheSceneTransitionStarted_) {
		return;
	}

	// シーン移行するか?
	if (selectSystem_->GetGotoGameScene()) {
		// 行きたいシーンへ
		requestSceneNo_ = kGame;
		hasTheSceneTransitionStarted_ = true;
	}
	else if (selectSystem_->GetGotoTitleScene()) {
		// 行きたいシーンへ
		requestSceneNo_ = kTitle;
		hasTheSceneTransitionStarted_ = true;
	}

}

void SelectScene::Draw()
{

	// スプライトの描画前処理
	Sprite::PreDraw(dxCommon_->GetCommadList());

	// セレクトシステムの描画
	selectSystem_->Draw();

	// スプライトの描画後処理
	Sprite::PostDraw();

}

void SelectScene::ModelCreate()
{
}

void SelectScene::TextureLoad()
{

	stagePhotTextureHandles_ = {};

	stageUITextureHandles_ = {};

}
