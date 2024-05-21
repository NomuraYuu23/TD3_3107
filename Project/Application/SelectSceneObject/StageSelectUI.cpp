#include "StageSelectUI.h"

// ステージ番号の大きさ
const Vector2 StageSelectUI::kStageNumSize;

void StageSelectUI::Initialize(const std::array<uint32_t, SpriteIndex::kSpriteIndexOfCount> textureHandles)
{

	// ステージ番号の大きさ
	spriteDatas_[kSpriteIndexStageNumber].sprite_->SetTextureSize(kStageNumSize);

}

void StageSelectUI::Update()
{

#ifdef _DEBUG
	ApplyGlobalVariables();
#endif // _DEBUG



}

void StageSelectUI::Draw(bool notMove)
{

	if (notMove) {
		spriteDatas_[kSpriteIndexLeftArrow].sprite_->Draw();
		spriteDatas_[kSpriteIndexRightArrow].sprite_->Draw();
		spriteDatas_[kSpriteIndexStage].sprite_->Draw();
		spriteDatas_[kSpriteIndexStageNumber].sprite_->Draw();
		spriteDatas_[kSpriteIndexOperation].sprite_->Draw();
	}

}

void StageSelectUI::Setting()
{
}

void StageSelectUI::SetStageNum(uint32_t stageNum)
{

	stageNum_ = stageNum;
	spriteDatas_[kSpriteIndexStageNumber].sprite_->SetTextureLeftTop(Vector2{ spriteDatas_[kSpriteIndexStageNumber].size_.x * stageNum, 0.0f });

}

void StageSelectUI::ApplyGlobalVariables()
{
}

void StageSelectUI::SpriteStruct::Initialize(
	uint32_t textureHandle, 
	const Vector2& postion, 
	const Vector2& size, 
	const Vector4& color)
{

	textureHandle_ = textureHandle;

	postion_ = postion;

	size_ = size;

	// スプライト作成
	sprite_.reset(Sprite::Create(textureHandle, postion, color));
	
	// スプライトの大きさ
	sprite_->SetSize(size_);

	// スプライトの表示部分
	sprite_->SetTextureSize(size_);

}
