#include "SelectScene.h"

void SelectScene::Initialize()
{

	IScene::Initialize();

	ModelCreate();
	TextureLoad();

	audioManager_ = std::make_unique<StageSelectAudioManager>();
	audioManager_->StaticInitialize();
	audioManager_->Initialize();

	selectSystem_ = std::make_unique<SelectSystem>();
	selectSystem_->Initialize(stagePhotTextureHandles_, stageUITextureHandles_, audioManager_.get());

	hasTheSceneTransitionStarted_ = false;

	// スカイドーム
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(skydomeModel_.get());

	// ビュープロジェクション
	camera_.Initialize();
	EulerTransform baseCameraTransform = {
		1.0f, 1.0f, 1.0f,
		0.58f,0.0f,0.0f,
		0.0f, 23.0f, -100.0f };
	camera_.SetTransform(baseCameraTransform);
	camera_.Update();

	FogManager* fogManager = FogManager::GetInstance();
	fogManager->SetColor({ 0.5f,0.5f,0.5f,0.5f });
	fogManager->SetNear(0.01f);
	fogManager->SetFar(0.0f);

	// ステージセレクトシーン用BGMの再生
	audioManager_->PlayWave(kStageSelectSceneBGM);

}

void SelectScene::Update()
{

	// スカイドーム
	skydome_->Update();

	if (requestSceneNo_ == kGame || isBeingReset_) {
		resetScene_ = false;
		// BGM音量下げる
		if (isDecreasingVolume) {
			LowerVolumeBGM();
		}
		return;
	}

	// システム更新
	selectSystem_->Update();

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
	preDrawDesc.environmentTextureHandle = skyboxTextureHandle_;

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

	skyboxTextureHandle_ = TextureManager::Load("Resources/default/rostock_laage_airport_4k.dds", DirectXCommon::GetInstance(), textureHandleManager_.get());

}

void SelectScene::LowerVolumeBGM()
{
	const uint32_t startHandleIndex = 2;

	uint32_t index = kStageSelectSceneBGM + startHandleIndex;

	for (uint32_t i = 0; i < audioManager_->kMaxPlayingSoundData; ++i) {
		if (audioManager_->GetPlayingSoundDatas()[i].handle_ == index) {
			float decreasingVolume = 1.0f / 60.0f;
			float volume = audioManager_->GetPlayingSoundDatas()[i].volume_ - decreasingVolume;
			if (volume < 0.0f) {
				volume = 0.0f;
				audioManager_->StopWave(i);
				isDecreasingVolume = false;
			}
			else {
				audioManager_->SetPlayingSoundDataVolume(i, volume);
				audioManager_->SetVolume(i, audioManager_->GetPlayingSoundDatas()[i].volume_);
			}
		}
	}

}
