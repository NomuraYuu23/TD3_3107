#include "StageSelectUI.h"
#include "../../Engine/GlobalVariables/GlobalVariables.h"
#include "../AllSceneObject/StageNumberManager.h"

// ステージ番号の大きさ
const Vector2 StageSelectUI::kStageNumSize = {128.0f, 128.0f};

void StageSelectUI::Initialize(const std::array<uint32_t, SpriteIndex::kSpriteIndexOfCount> textureHandles)
{

	// 位置
	Vector2 position = { 0.0f,0.0f };

	// サイズ
	Vector2 size = { 1.0f,1.0f };

	// 色
	Vector4 color = { 1.0f,1.0f,1.0f,1.0f };

	for (uint32_t i = 0; i < kSpriteIndexOfCount; ++i) {
		spriteDatas_[i].Initialize(textureHandles[i], position, size, color);
	}

	// ステージ番号の大きさ
	spriteDatas_[kSpriteIndexStageNumber].sprite_->SetTextureSize(kStageNumSize);

	SetStageNum();

#pragma region 調整項目クラス
	// 調整項目クラスのインスタンス取得
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	// グループ名設定
	const char* groupName = "StageSelectUI";
	// 指定した名前でグループ追加
	globalVariables->CreateGroup(groupName);

	// メンバ変数の調整したい項目をグローバル変数に追加

	globalVariables->AddItem(groupName, "leftPostion", spriteDatas_[kSpriteIndexLeftArrow].postion_);
	globalVariables->AddItem(groupName, "leftSize", spriteDatas_[kSpriteIndexLeftArrow].size_);

	globalVariables->AddItem(groupName, "rightPostion", spriteDatas_[kSpriteIndexRightArrow].postion_);
	globalVariables->AddItem(groupName, "rightSize", spriteDatas_[kSpriteIndexRightArrow].size_);

	globalVariables->AddItem(groupName, "stageSelectPostion", spriteDatas_[kSpriteIndexStage].postion_);
	globalVariables->AddItem(groupName, "stageSelectSize", spriteDatas_[kSpriteIndexStage].size_);

	globalVariables->AddItem(groupName, "stageNumberPostion", spriteDatas_[kSpriteIndexStageNumber].postion_);
	globalVariables->AddItem(groupName, "stageNumberSize", spriteDatas_[kSpriteIndexStageNumber].size_);

	globalVariables->AddItem(groupName, "stageUiPostion", spriteDatas_[kSpriteIndexOperation].postion_);
	globalVariables->AddItem(groupName, "stageUiSize", spriteDatas_[kSpriteIndexOperation].size_);
	
	globalVariables->AddItem(groupName, "practiceUIPostion", spriteDatas_[kSpriteIndexPracticeStage].postion_);
	globalVariables->AddItem(groupName, "practiceUISize", spriteDatas_[kSpriteIndexPracticeStage].size_);

	ApplyGlobalVariables();

#pragma endregion

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
		if (StageNumberManager::stageNum_ != 0) { // ステージ番号が0以外のときのみ
			spriteDatas_[kSpriteIndexStage].sprite_->Draw();
			spriteDatas_[kSpriteIndexStageNumber].sprite_->Draw();
		}
		else { // 練習場描画
			spriteDatas_[kSpriteIndexPracticeStage].sprite_->Draw();
		}
		spriteDatas_[kSpriteIndexOperation].sprite_->Draw();
	}

}

void StageSelectUI::SetStageNum()
{

	spriteDatas_[kSpriteIndexStageNumber].sprite_->SetTextureLeftTop(Vector2{ kStageNumSize.x * (StageNumberManager::stageNum_), 0.0f});

}

void StageSelectUI::ApplyGlobalVariables()
{

	// 調整項目クラスのインスタンス取得
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	// グループ名の設定
	const char* groupName = "StageSelectUI";

	spriteDatas_[kSpriteIndexLeftArrow].postion_ = globalVariables->GetVector2Value(groupName, "leftPostion");
	spriteDatas_[kSpriteIndexLeftArrow].size_ = globalVariables->GetVector2Value(groupName, "leftSize");

	spriteDatas_[kSpriteIndexRightArrow].postion_ = globalVariables->GetVector2Value(groupName, "rightPostion");
	spriteDatas_[kSpriteIndexRightArrow].size_ = globalVariables->GetVector2Value(groupName, "rightSize");

	spriteDatas_[kSpriteIndexStage].postion_ = globalVariables->GetVector2Value(groupName, "stageSelectPostion");
	spriteDatas_[kSpriteIndexStage].size_ = globalVariables->GetVector2Value(groupName, "stageSelectSize");

	spriteDatas_[kSpriteIndexStageNumber].postion_ = globalVariables->GetVector2Value(groupName, "stageNumberPostion");
	spriteDatas_[kSpriteIndexStageNumber].size_ = globalVariables->GetVector2Value(groupName, "stageNumberSize");

	spriteDatas_[kSpriteIndexOperation].postion_ = globalVariables->GetVector2Value(groupName, "stageUiPostion");
	spriteDatas_[kSpriteIndexOperation].size_ = globalVariables->GetVector2Value(groupName, "stageUiSize");

	spriteDatas_[kSpriteIndexPracticeStage].postion_ = globalVariables->GetVector2Value(groupName, "practiceUIPostion");
	spriteDatas_[kSpriteIndexPracticeStage].size_ = globalVariables->GetVector2Value(groupName, "practiceUISize");

	// ここからスプライトに反映

	spriteDatas_[kSpriteIndexLeftArrow].sprite_->SetPosition(spriteDatas_[kSpriteIndexLeftArrow].postion_);
	spriteDatas_[kSpriteIndexLeftArrow].sprite_->SetSize(spriteDatas_[kSpriteIndexLeftArrow].size_);

	spriteDatas_[kSpriteIndexRightArrow].sprite_->SetPosition(spriteDatas_[kSpriteIndexRightArrow].postion_);
	spriteDatas_[kSpriteIndexRightArrow].sprite_->SetSize(spriteDatas_[kSpriteIndexRightArrow].size_);

	spriteDatas_[kSpriteIndexStage].sprite_->SetPosition(spriteDatas_[kSpriteIndexStage].postion_);
	spriteDatas_[kSpriteIndexStage].sprite_->SetSize(spriteDatas_[kSpriteIndexStage].size_);
	
	spriteDatas_[kSpriteIndexStageNumber].sprite_->SetPosition(spriteDatas_[kSpriteIndexStageNumber].postion_);
	spriteDatas_[kSpriteIndexStageNumber].sprite_->SetSize(spriteDatas_[kSpriteIndexStageNumber].size_);

	spriteDatas_[kSpriteIndexOperation].sprite_->SetPosition(spriteDatas_[kSpriteIndexOperation].postion_);
	spriteDatas_[kSpriteIndexOperation].sprite_->SetSize(spriteDatas_[kSpriteIndexOperation].size_);

	spriteDatas_[kSpriteIndexPracticeStage].sprite_->SetPosition(spriteDatas_[kSpriteIndexPracticeStage].postion_);
	spriteDatas_[kSpriteIndexPracticeStage].sprite_->SetSize(spriteDatas_[kSpriteIndexPracticeStage].size_);
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

}
