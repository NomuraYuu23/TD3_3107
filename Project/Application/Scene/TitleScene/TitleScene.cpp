#include "TitleScene.h"
#include "../../../Engine/base/TextureManager.h"
#include "../../../Engine/2D/ImguiManager.h"
#include "../../../Engine/GlobalVariables/GlobalVariables.h"
#include "../../../Engine/Math/Ease.h"
#include "../../../Engine/3D/ModelDraw.h"

TitleScene::~TitleScene()
{
	if (stopAudio_) {
		for (uint32_t i = 0; i < audioManager_->kMaxPlayingSoundData; ++i) {
			audioManager_->StopWave(i);
		}
	}
}

void TitleScene::Initialize()
{

	IScene::Initialize();

	ModelCreate();
	TextureLoad();

	audioManager_ = std::make_unique<TitleAudioManager>();
	audioManager_->StaticInitialize();
	audioManager_->Initialize();

	// ビュープロジェクション
	EulerTransform baseCameraTransform = {
		1.0f, 1.0f, 1.0f,
		0.0f,0.0f,0.0f,
		0.0f, 0.0f, -35.0f };
	camera_.SetTransform(baseCameraTransform);

	// スカイドーム
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(skydomeModel_.get());

	// スプライト生成
	titleSprite_.reset(Sprite::Create(logoTexHandle_, { 640.0f,270.0f }, { 1.0f,1.0f,1.0f,1.0f }));
	buttonSprite_.reset(Sprite::Create(buttonTexHandle_, { 640.0f,480.0f }, { 1.0f,1.0f,1.0f,1.0f }));

	titleSprite_->SetAnchorPoint({ 0.5f, 0.5f });
	buttonSprite_->SetAnchorPoint({ 0.5f, 0.5f });

	// デバッグ以外の場合行う
	#ifndef _DEBUG

	// タイトルシーン用BGMの再生
	audioManager_->PlayWave(kTitleSceneBGM);

	#endif // !_DEBUG
}

void TitleScene::Update()
{

#ifdef _DEBUG
	ImguiDraw();
#endif // _DEBUG

	if ((input_->TriggerJoystick(JoystickButton::kJoystickButtonA) || input_->TriggerKey(DIK_SPACE)) &&
		requestSceneNo_ == kTitle) {
		// 行きたいシーンへ
		requestSceneNo_ = kSelect;
	}

	if (requestSceneNo_ == kSelect || isBeingReset_) {
		resetScene_ = false;
		// BGM音量下げる
		if (isDecreasingVolume) {
			LowerVolumeBGM();
		}
		return;
	}
	
	// カメラ
	camera_.Update();
	
	// スカイドーム
	skydome_->Update();

}

void TitleScene::Draw()
{

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(dxCommon_->GetCommadList());

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	renderTargetTexture_->ClearDepthBuffer();


#pragma endregion

	ModelDraw::PreDrawDesc preDrawDesc;
	preDrawDesc.commandList = dxCommon_->GetCommadList();
	preDrawDesc.directionalLight = directionalLight_.get();
	preDrawDesc.fogManager = FogManager::GetInstance();
	preDrawDesc.pointLightManager = pointLightManager_.get();
	preDrawDesc.spotLightManager = spotLightManager_.get();
	preDrawDesc.environmentTextureHandle = skyboxTextureHandle_;

	ModelDraw::PreDraw(preDrawDesc);

	//3Dオブジェクトはここ

	// スカイドーム
	if (isDrawSkydome_) {
		skydome_->Draw(camera_);
	}

	ModelDraw::PostDraw();

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(dxCommon_->GetCommadList());

	//背景
	//前景スプライト描画

	titleSprite_->Draw();
	buttonSprite_->Draw();

	// 前景スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion

}

void TitleScene::ImguiDraw()
{

#ifdef _DEBUG
	ImGui::Begin("Title");
	ImGui::Checkbox("isDrawSkydome", &isDrawSkydome_);
	ImGui::End();
#endif // _DEBUG

}

void TitleScene::ModelCreate()
{

	// スカイドーム
	skydomeModel_.reset(Model::Create("Resources/Model/Skydome/", "skydome.obj", dxCommon_, textureHandleManager_.get()));

}

void TitleScene::TextureLoad()
{

	logoTexHandle_ = TextureManager::Load("Resources/UI/Title/TitleLogo.png", DirectXCommon::GetInstance(), textureHandleManager_.get());
	buttonTexHandle_ = TextureManager::Load("Resources/UI/Title/Button.png", DirectXCommon::GetInstance(), textureHandleManager_.get());

	skyboxTextureHandle_ = TextureManager::Load("Resources/default/rostock_laage_airport_4k.dds", DirectXCommon::GetInstance(), textureHandleManager_.get());


}

void TitleScene::LowerVolumeBGM()
{

	const uint32_t startHandleIndex = 3;

	float decreasingVolume = 1.0f / 60.0f;
	float volume = audioManager_->GetPlayingSoundDatas()[kTitleSceneBGM].volume_ - decreasingVolume;
	if (volume < 0.0f) {
		volume = 0.0f;
		audioManager_->StopWave(kTitleSceneBGM);
		isDecreasingVolume = false;
	}
	else {
		audioManager_->SetPlayingSoundDataVolume(kTitleSceneBGM, volume);
		audioManager_->SetVolume(kTitleSceneBGM, audioManager_->GetPlayingSoundDatas()[kTitleSceneBGM].volume_);
	}

}
