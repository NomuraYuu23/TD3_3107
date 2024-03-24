#include "TitleScene.h"
#include "../../../Engine/base/TextureManager.h"
#include "../../../Engine/2D/ImguiManager.h"
#include "../../../Engine/GlobalVariables/GlobalVariables.h"
#include "../../../Engine/Math/Ease.h"

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
	TransformStructure baseCameraTransform = {
		1.0f, 1.0f, 1.0f,
		0.0f,0.0f,0.0f,
		0.0f, 0.0f, -35.0f };
	camera_.SetTransform(baseCameraTransform);

	// 平行光源
	directionalLight_ = std::make_unique<DirectionalLight>();
	directionalLight_->Initialize();

	// スカイドーム
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(skydomeModel_.get());

	// 槍
	spear_ = std::make_unique<SampleObject>();
	spear_->Initialize(spearModel_.get());

	//アウトライン
	outline_.Initialize();
	outline_.Map();
}

void TitleScene::Update()
{

#ifdef _DEBUG
	ImguiDraw();
#endif // _DEBUG

	if ((input_->TriggerJoystick(JoystickButton::kJoystickButtonA) || input_->TriggerKey(DIK_SPACE)) &&
		requestSceneNo == kTitle) {
		// 行きたいシーンへ
		requestSceneNo = kGame;
	}

	// BGM音量下げる
	if (requestSceneNo == kTutorial && isDecreasingVolume) {
		LowerVolumeBGM();
	}
	
	// カメラ
	camera_.Update();
	
	// 光源向き変更用ImGui
	ImGui::Begin("光源");
	ImGui::DragFloat3("光源向き", &direction.x, 0.01f, -1.0f, 1.0f);
	ImGui::End();

	//光源
	DirectionalLightData directionalLightData;
	directionalLightData.color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLightData.direction = Vector3::Normalize(direction);
	directionalLightData.intencity = intencity;
	directionalLight_->Update(directionalLightData);

	// スカイドーム
	skydome_->Update();

	// 槍
	spear_->Update();
	// ImGui表示
	spear_->ImGuiDraw();

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

	Model::PreDraw(dxCommon_->GetCommadList());

	//3Dオブジェクトはここ

	//光源
	directionalLight_->Draw(dxCommon_->GetCommadList(), 6);

	// スカイドーム
	if (isDrawSkydome_) {
		skydome_->Draw(camera_);
	}

	// 槍
	spear_->Draw(camera_);

	Model::PostDraw();
	Model::PreDrawOutLine(dxCommon_->GetCommadList());

	Model::PostDraw();
#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(dxCommon_->GetCommadList());

	//背景
	//前景スプライト描画

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

	// 槍
	spearModel_.reset(Model::Create("Resources/Model/Player/", "Player.gltf", dxCommon_, textureHandleManager_.get()));
}

void TitleScene::TextureLoad()
{

}

void TitleScene::LowerVolumeBGM()
{

	//for (uint32_t i = 0; i < audioManager_->kMaxPlayingSoundData; ++i) {
	//	if (audioManager_->GetPlayingSoundDatas()[i].handle_ == kTitleAudioNameIndexBGM) {
	//		float decreasingVolume = 1.0f / 60.0f;
	//		float volume = audioManager_->GetPlayingSoundDatas()[i].volume_ - decreasingVolume;
	//		if (volume < 0.0f) {
	//			volume = 0.0f;
	//			audioManager_->StopWave(i);
	//			isDecreasingVolume = false;
	//		}
	//		else {
	//			audioManager_->SetPlayingSoundDataVolume(i, volume);
	//			audioManager_->SetVolume(i, audioManager_->GetPlayingSoundDatas()[i].volume_);
	//		}
	//	}
	//}

}
