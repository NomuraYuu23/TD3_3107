#include "TitleScene.h"
#include "../../../Engine/base/TextureManager.h"
#include "../../../Engine/2D/ImguiManager.h"
#include "../../../Engine/GlobalVariables/GlobalVariables.h"
#include "../../../Engine/Math/Ease.h"
#include "../../../Engine/3D/ModelDraw.h"
#include "../../../Engine/Particle/ParticleManager.h"
#include "../../Particle/EmitterName.h"
#include "../../../Engine//Math/DeltaTime.h"

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

	//パーティクル
	particleManager_ = ParticleManager::GetInstance();
	particleManager_->Finalize();
	std::array<Model*, ParticleModelIndex::kCountofParticleModelIndex> particleModel;
	particleModel[ParticleModelIndex::kUvChecker] = particleUvcheckerModel_.get();
	particleModel[ParticleModelIndex::kCircle] = particleCircleModel_.get();
	particleModel[ParticleModelIndex::kBambooLeaf] = particleLeafModel_.get();
	particleModel[ParticleModelIndex::kSpearLeaf] = particleSpearLeafModel_.get();
	particleModel[ParticleModelIndex::kSmoke] = particleSmokeModel_.get();
	particleManager_->ModelCreate(particleModel);

	// スカイドーム
	skydomeModel_->SetTextureHandle(skyDomeTexHandle_, 0);
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(skydomeModel_.get());

	// スプライト生成
	titleSprite_.reset(Sprite::Create(logoTexHandle_, { 640.0f,270.0f }, { 1.0f,1.0f,1.0f,1.0f }));
	buttonSprite_.reset(Sprite::Create(buttonTexHandle_, { 640.0f,550.0f }, { 1.0f,1.0f,1.0f,1.0f }));

	// 演出用座標を初期化
	prevPos_ = buttonSprite_->GetPosition();
	postPos_ = { prevPos_.x, prevPos_.y + 10.0f };

	// タイトルシーン用BGMの再生
	audioManager_->PlayWave(kTitleSceneBGM);

	// デバック以外の場合行う
#ifndef _DEBUG

	// トランスフォーム生成
	emitTransform_ = std::make_unique<EulerTransform>();
	emitTransform_->translate = {0.0f, 0.0f, 0.0f};
	emitTransform_->scale = {1.0f, 1.0f, 1.0f };
	emitTransform_->rotate = { 0.0f, 0.0f, 0.0f };
	emitTransform_->translate.y -= 15.0f;
	emitTransform_->translate.z += 5.0f;

	// ここで環境パーティクルの再生を行う
	fallingLeafDesc_.transform = emitTransform_.get();
	fallingLeafDesc_.instanceCount = 3;
	fallingLeafDesc_.frequency = 0.25f;
	fallingLeafDesc_.lifeTime = 5.0f;
	fallingLeafDesc_.particleModelNum = kBambooLeaf;
	fallingLeafDesc_.paeticleName = kFallingLeafParticle;
	fallingLeafDesc_.velocity = { -1.0f, -1.0f, 0.0f };

	// 無限生成エミッタで生成し続ける
	ParticleManager::GetInstance()->MakeEmitter(&fallingLeafDesc_, EmitterName::kInfiniteEmitter);

#endif // !_DEBUG

}

void TitleScene::Update()
{

#ifdef _DEBUG
	ImguiDraw();
#endif // _DEBUG

	if ((input_->TriggerJoystick(JoystickButton::kJoystickButtonA) || input_->TriggerKey(DIK_SPACE)) &&
		requestSceneNo_ == kTitle) {
		// ゲーム開始SEの再生
		audioManager_->PlayWave(kStartSE);
		
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

	/// ボタンスプライト用演出
	// 戻すか
	if (isReturn_) {
		if (currentButtonStagingTime_ < ButtonStagingTime_) {
			Vector2 pos = Ease::Easing(Ease::EaseName::EaseInOutQuad, prevPos_, postPos_, (currentButtonStagingTime_ / ButtonStagingTime_));

			buttonSprite_->SetPosition(pos);

			// 経過秒数分加算
			currentButtonStagingTime_ += kDeltaTime_;
		}
		else {
			currentButtonStagingTime_ = 0.0f;
			isReturn_ = false;
		}

	}else{
		if (currentButtonStagingTime_ < ButtonStagingTime_) {
			Vector2 pos = Ease::Easing(Ease::EaseName::EaseInOutQuad, postPos_, prevPos_, (currentButtonStagingTime_ / ButtonStagingTime_));

			buttonSprite_->SetPosition(pos);

			// 経過秒数分加算
			currentButtonStagingTime_ += kDeltaTime_;
		}
		else {
			currentButtonStagingTime_ = 0.0f;
			isReturn_ = true;
		}
	}


	//パーティクル
	particleManager_->Update(camera_);

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

	// パーティクルはここ
	particleManager_->Draw(camera_.GetViewProjectionMatrix(), dxCommon_->GetCommadList());

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
	// パーティクル
	particleUvcheckerModel_.reset(Model::Create("Resources/default/", "plane.gltf", dxCommon_, textureHandleManager_.get()));
	particleCircleModel_.reset(Model::Create("Resources/Particle/", "plane.obj", dxCommon_, textureHandleManager_.get()));
	particleLeafModel_.reset(Model::Create("Resources/Particle/BambooLeaf", "BambooLeaf.obj", dxCommon_, textureHandleManager_.get()));
	particleSpearLeafModel_.reset(Model::Create("Resources/Particle/SpearLeaf", "SpearLeaf.obj", dxCommon_, textureHandleManager_.get()));
	particleSmokeModel_.reset(Model::Create("Resources/Particle/Smoke", "Smoke.obj", dxCommon_, textureHandleManager_.get()));

	// スカイドーム
	skydomeModel_.reset(Model::Create("Resources/Model/Skydome/", "skydome.obj", dxCommon_, textureHandleManager_.get()));

}

void TitleScene::TextureLoad()
{

	skyDomeTexHandle_ = TextureManager::Load("Resources/Model/SkyDome/SkyTex.png", DirectXCommon::GetInstance(), textureHandleManager_.get());

	logoTexHandle_ = TextureManager::Load("Resources/UI/Title/TitleLogo.png", DirectXCommon::GetInstance(), textureHandleManager_.get());
	buttonTexHandle_ = TextureManager::Load("Resources/UI/Title/Button.png", DirectXCommon::GetInstance(), textureHandleManager_.get());

	skyboxTextureHandle_ = TextureManager::Load("Resources/default/rostock_laage_airport_4k.dds", DirectXCommon::GetInstance(), textureHandleManager_.get());


}

void TitleScene::LowerVolumeBGM()
{

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
