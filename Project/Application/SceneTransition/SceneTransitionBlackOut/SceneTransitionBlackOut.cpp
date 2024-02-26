#include "SceneTransitionBlackOut.h"
#include "../../../Engine/base/TextureManager.h"
#include "../../../Engine/base/DirectXCommon.h"
#include "../../../Engine/base/WinApp.h"

SceneTransitionBlackOut::~SceneTransitionBlackOut()
{

	//ISceneTransition::~ISceneTransition();

}

void SceneTransitionBlackOut::Initialize()
{

	ISceneTransition::Initialize();

	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("Resources/default/white2x2.png", DirectXCommon::GetInstance(), textureHandleManager_.get());
	color_ = { 0.0f,0.0f,0.0f,0.0f };
	Vector2 position = { static_cast<float>(WinApp::kWindowWidth) / 2.0f, static_cast<float>(WinApp::kWindowHeight) / 2.0f };
	sprite_.reset(Sprite::Create(textureHandle_, position, color_));
	sprite_->SetSize(Vector2{ static_cast<float>(WinApp::kWindowWidth), static_cast<float>(WinApp::kWindowHeight) });

	// テクスチャ読み込み
	loadTextureHandle_ = TextureManager::Load("Resources/default/load.png", DirectXCommon::GetInstance(), textureHandleManager_.get());
	position = { 1100.0f, 600.0f };
	loadSprite_.reset(Sprite::Create(loadTextureHandle_, position, color_));
	loadSprite_->SetSize({ 128.0f, 128.0f });
	loadSprite_->SetTextureSize({ 128.0f, 128.0f });
	loadSprite_->SetTextureLeftTop({ 0.0f, 0.0f });
	loadCount_ = 0;

}

void SceneTransitionBlackOut::Update()
{

	ISceneTransition::Update();

	// α値変更
	if (isFadeIn_) {
		color_.w = fadeTimer_ / fadeInTime_;
	}
	else {
		color_.w = 1.0f - fadeTimer_ / fadeOutTime_;
	}

	loadCount_ = loadCount_ + 2 % 80;
	loadSprite_->SetTextureLeftTop({ 128.0f * (static_cast<float>(loadCount_ / 10)), 0.0f });

	sprite_->SetColor(color_);
	loadSprite_->SetColor({ 1.0f,1.0f,1.0f,color_.w });

}

void SceneTransitionBlackOut::Draw()
{

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(DirectXCommon::GetInstance()->GetCommadList());

	sprite_->Draw();
	loadSprite_->Draw();

	// 前景スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion

}
