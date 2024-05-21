#include "StagePhot.h"
#include "../../Engine/GlobalVariables/GlobalVariables.h"
#include <numbers>
#include "../../Engine/Math/Ease.h"

void StagePhot::Initialize(const std::array<uint32_t, kStageMax>& stagePhotTextureHandles)
{


	// テクスチャハンドル
	textureHandles_ = stagePhotTextureHandles;

	// 固定位置
	positions_[0] = { -960.0f, 432.0f };
	positions_[1] = { -160.0f ,  432.0f };
	positions_[2] = { 640.0f, 360.0f }; //中央
	positions_[3] = { 1440.0f,  432.0f };
	positions_[4] = { 2240.0f,  432.0f };

	// サイズ
	sizes_[0] = { 512.0f, 360.f };
	sizes_[1] = { 512.0f, 360.f };
	sizes_[2] = { 768.0f, 432.0f }; //中央
	sizes_[3] = { 512.0f, 360.f };
	sizes_[4] = { 512.0f, 360.f };

	// スプライト
	Vector4 color = { 1.0f,1.0f,1.0f,1.0f };
	Vector2 anchorPoint = { 0.5f, 0.5f };
	for (size_t i = 0; i < kSpriteNum; i++) {
		sprite_[i].reset(Sprite::Create(textureHandles_[0], positions_[i], color));
		sprite_[i]->SetSize(sizes_[i]);
		sprite_[i]->Update();
	}

#pragma region 調整項目クラス
	// 調整項目クラスのインスタンス取得
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	// グループ名設定
	const char* groupName = "StagePhot";
	// 指定した名前でグループ追加
	globalVariables->CreateGroup(groupName);

	// メンバ変数の調整したい項目をグローバル変数に追加

	//移動中間スケール
	globalVariables->AddItem(groupName, "offScreenLeftPos", positions_[0]);
	globalVariables->AddItem(groupName, "screenLeftPos", positions_[1]);
	globalVariables->AddItem(groupName, "screenCenterPos", positions_[2]);
	globalVariables->AddItem(groupName, "screenRightPos", positions_[3]);
	globalVariables->AddItem(groupName, "offScreenRightPos", positions_[4]);

	// サイズ
	globalVariables->AddItem(groupName, "offScreenLeftSize", sizes_[0]);
	globalVariables->AddItem(groupName, "screenLeftSize", sizes_[1]);
	globalVariables->AddItem(groupName, "screenCenterSize", sizes_[2]);
	globalVariables->AddItem(groupName, "screenRightSize", sizes_[3]);
	globalVariables->AddItem(groupName, "offScreenRightSize", sizes_[4]);

	// 振幅
	globalVariables->AddItem(groupName, "amplitude", amplitude_);
	// フレーム
	globalVariables->AddItem(groupName, "fre", fre_);

	ApplyGlobalVariables();

#pragma endregion

	centerPositions_ = positions_[2];

	Setting(0);

}

void StagePhot::Update()
{

#ifdef _DEBUG
	ApplyGlobalVariables();
#endif // _DEBUG


	waveAnimation_t_ += 0.01f;
	if (waveAnimation_t_ >= 1.0f) {
		waveAnimation_t_ -= 2.0f;
	}
	waveVelocity_.y = amplitude_ * std::cosf(1.0f * float(std::numbers::pi) * fre_ * waveAnimation_t_);

	positions_[2] = centerPositions_+ waveVelocity_;
	sprite_[2]->SetPosition(positions_[2]);
	sprite_[2]->Update();

}

void StagePhot::Draw()
{

	for (size_t i = 0; i < kSpriteNum; i++) {
		sprite_[i]->Draw();
	}

}

void StagePhot::Setting(size_t stageNum)
{

	// ステージナンバー
	stageNum_ = stageNum;
	// スプライトのテクスチャ
	TextureHandleChange();

}

void StagePhot::MoveRight(float t)
{

	for (size_t i = 1; i < kSpriteNum; i++) {
		sprite_[i]->SetPosition(Ease::Easing(Ease::EaseName::EaseInQuad, positions_[i], positions_[i - 1], t));
		sprite_[i]->SetSize(Ease::Easing(Ease::EaseName::EaseInQuad, sizes_[i], sizes_[i - 1], t));
		sprite_[i]->Update();
	}
	if (t >= 1.0f) {
		stageNum_++;
		if (kStageMax == stageNum_) {
			stageNum_ = 0;
		}
		TextureHandleChange();
	}

}

void StagePhot::MoveLeft(float t)
{

	for (size_t i = 0; i < 4; i++) {
		sprite_[i]->SetPosition(Ease::Easing(Ease::EaseName::EaseInQuad, positions_[i], positions_[i + 1], t));
		sprite_[i]->SetSize(Ease::Easing(Ease::EaseName::EaseInQuad, sizes_[i], sizes_[i + 1], t));
		sprite_[i]->Update();
	}
	if (t >= 1.0f) {
		stageNum_--;
		if (-1 == stageNum_) {
			stageNum_ = kStageMax - 1;
		}
		TextureHandleChange();
	}

}

void StagePhot::TextureHandleChange()
{

	// stageNum_ == 0
	if (stageNum_ == 0) {
		sprite_[0]->SetTextureHandle(textureHandles_[kStageMax - 2]);
		sprite_[1]->SetTextureHandle(textureHandles_[kStageMax - 1]);
		sprite_[2]->SetTextureHandle(textureHandles_[stageNum_]);
		sprite_[3]->SetTextureHandle(textureHandles_[stageNum_ + 1]);
		sprite_[4]->SetTextureHandle(textureHandles_[stageNum_ + 2]);
	}
	// stageNum_ == 1
	else if (stageNum_ == 1) {
		sprite_[0]->SetTextureHandle(textureHandles_[kStageMax - 1]);
		sprite_[1]->SetTextureHandle(textureHandles_[stageNum_ - 1]);
		sprite_[2]->SetTextureHandle(textureHandles_[stageNum_]);
		sprite_[3]->SetTextureHandle(textureHandles_[stageNum_ + 1]);
		sprite_[4]->SetTextureHandle(textureHandles_[stageNum_ + 2]);
	}
	// stageNum_ == stageMax_- 2
	else if (stageNum_ == kStageMax - 2) {
		sprite_[0]->SetTextureHandle(textureHandles_[stageNum_ - 2]);
		sprite_[1]->SetTextureHandle(textureHandles_[stageNum_ - 1]);
		sprite_[2]->SetTextureHandle(textureHandles_[stageNum_]);
		sprite_[3]->SetTextureHandle(textureHandles_[stageNum_ + 1]);
		sprite_[4]->SetTextureHandle(textureHandles_[0]);
	}
	// stageNum_ == stageMax_- 1
	else if (stageNum_ == kStageMax - 1) {
		sprite_[0]->SetTextureHandle(textureHandles_[stageNum_ - 2]);
		sprite_[1]->SetTextureHandle(textureHandles_[stageNum_ - 1]);
		sprite_[2]->SetTextureHandle(textureHandles_[stageNum_]);
		sprite_[3]->SetTextureHandle(textureHandles_[0]);
		sprite_[4]->SetTextureHandle(textureHandles_[1]);
	}
	// それ以外
	else {
		sprite_[0]->SetTextureHandle(textureHandles_[stageNum_ - 2]);
		sprite_[1]->SetTextureHandle(textureHandles_[stageNum_ - 1]);
		sprite_[2]->SetTextureHandle(textureHandles_[stageNum_]);
		sprite_[3]->SetTextureHandle(textureHandles_[stageNum_ + 1]);
		sprite_[4]->SetTextureHandle(textureHandles_[stageNum_ + 2]);
	}

	for (size_t i = 0; i < 5; i++) {
		sprite_[i]->SetPosition(positions_[i]);
		sprite_[i]->SetSize(sizes_[i]);
		sprite_[i]->Update();
	}

}

void StagePhot::ApplyGlobalVariables()
{

	// 調整項目クラスのインスタンス取得
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	// グループ名の設定
	const char* groupName = "StagePhot";

	positions_[0] = globalVariables->GetVector2Value(groupName, "offScreenLeftPos");
	positions_[1] = globalVariables->GetVector2Value(groupName, "screenLeftPos");
	positions_[2] = globalVariables->GetVector2Value(groupName, "screenCenterPos");
	positions_[3] = globalVariables->GetVector2Value(groupName, "screenRightPos");
	positions_[4] = globalVariables->GetVector2Value(groupName, "offScreenRightPos");

	// サイズ
	sizes_[0] = globalVariables->GetVector2Value(groupName, "offScreenLeftSize");
	sizes_[1] = globalVariables->GetVector2Value(groupName, "screenLeftSize");
	sizes_[2] = globalVariables->GetVector2Value(groupName, "screenCenterSize");
	sizes_[3] = globalVariables->GetVector2Value(groupName, "screenRightSize");
	sizes_[4] = globalVariables->GetVector2Value(groupName, "offScreenRightSize");

	// 振幅
	amplitude_ = globalVariables->GetFloatValue(groupName, "amplitude");
	// フレーム
	fre_ = globalVariables->GetFloatValue(groupName, "fre");

}
