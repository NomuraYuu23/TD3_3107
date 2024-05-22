#include "SelectScene.h"

void SelectScene::Initialize()
{

	IScene::Initialize();

	ModelCreate();
	TextureLoad();

	selectSystem_ = std::make_unique<SelectSystem>();
	selectSystem_->Initialize(stagePhotTextureHandles_, stageUITextureHandles_);

	hasTheSceneTransitionStarted_ = false;

	// スカイドーム
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(skydomeModel_.get());

	// ビュープロジェクション
	EulerTransform baseCameraTransform = {
		1.0f, 1.0f, 1.0f,
		0.58f,0.0f,0.0f,
		0.0f, 23.0f, -100.0f };
	camera_.SetTransform(baseCameraTransform);

}

void SelectScene::Update()
{

	// システム更新
	selectSystem_->Update();

	// スカイドーム
	skydome_->Update();

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

	ModelDraw::PreDrawDesc preDrawDesc;
	preDrawDesc.commandList = dxCommon_->GetCommadList();
	preDrawDesc.directionalLight = directionalLight_.get();
	preDrawDesc.fogManager = FogManager::GetInstance();
	preDrawDesc.pointLightManager = pointLightManager_.get();
	preDrawDesc.spotLightManager = spotLightManager_.get();

	ModelDraw::PreDraw(preDrawDesc);

	// スカイドーム
	skydome_->Draw(camera_);

	ModelDraw::PostDraw();

	// スプライトの描画前処理
	Sprite::PreDraw(dxCommon_->GetCommadList());

	// セレクトシステムの描画
	selectSystem_->Draw();

	// スプライトの描画後処理
	Sprite::PostDraw();

}

void SelectScene::ModelCreate()
{

	skydomeModel_.reset(Model::Create("Resources/Model/Skydome/", "skydome.obj", dxCommon_, textureHandleManager_.get()));

}

void SelectScene::TextureLoad()
{

	stagePhotTextureHandles_ = {
		TextureManager::Load("Resources/SelectObject/Phot/Phot_01.png", DirectXCommon::GetInstance(), textureHandleManager_.get()),
		TextureManager::Load("Resources/SelectObject/Phot/Phot_02.png", DirectXCommon::GetInstance(), textureHandleManager_.get()),
		TextureManager::Load("Resources/SelectObject/Phot/Phot_03.png", DirectXCommon::GetInstance(), textureHandleManager_.get()),
		TextureManager::Load("Resources/SelectObject/Phot/Phot_04.png", DirectXCommon::GetInstance(), textureHandleManager_.get()),
		TextureManager::Load("Resources/SelectObject/Phot/Phot_05.png", DirectXCommon::GetInstance(), textureHandleManager_.get())
	};

	stageUITextureHandles_ = {
		TextureManager::Load("Resources/SelectObject/UI/LeftArrow.png", DirectXCommon::GetInstance(), textureHandleManager_.get()),
		TextureManager::Load("Resources/SelectObject/UI/RightArrow.png", DirectXCommon::GetInstance(), textureHandleManager_.get()),
		TextureManager::Load("Resources/SelectObject/UI/Stage.png", DirectXCommon::GetInstance(), textureHandleManager_.get()),
		TextureManager::Load("Resources/SelectObject/UI/StageNumber.png", DirectXCommon::GetInstance(), textureHandleManager_.get()),
		TextureManager::Load("Resources/SelectObject/UI/Operation.png", DirectXCommon::GetInstance(), textureHandleManager_.get())
	};

}
