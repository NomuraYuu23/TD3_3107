#include "SelectScene.h"

void SelectScene::Initialize()
{

	IScene::Initialize();

	ModelCreate();
	TextureLoad();

	selectSystem_ = std::make_unique<SelectSystem>();
	selectSystem_->Initialize(stagePhotTextureHandles_, stageUITextureHandles_);

}

void SelectScene::Update()
{

	if (input_->TriggerKey(DIK_SPACE)) {
		// 行きたいシーンへ
		requestSceneNo_ = kGame;
	}

}

void SelectScene::Draw()
{
}

void SelectScene::ModelCreate()
{
}

void SelectScene::TextureLoad()
{

	stagePhotTextureHandles_ = {

	}

}
