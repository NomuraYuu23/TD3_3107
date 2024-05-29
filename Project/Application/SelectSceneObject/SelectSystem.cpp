#include "SelectSystem.h"

void SelectSystem::Initialize(const std::array<uint32_t, StageNumberManager::kStageMax>& stagePhotTextureHandles, const std::array<uint32_t, StageSelectUI::SpriteIndex::kSpriteIndexOfCount>& stageUITextureHandles, StageSelectAudioManager* sam)
{
	// 入力
	input_ = Input::GetInstance();

	// オーディオマネージャー
	audioManager_ = sam;

	// 動いているか
	isMoveRight_ = false;
	isMoveLeft_ = false;

	// イージング
	easeTimer_ = 0.0f;
	easeSpeed_ = 0.05f;

	// ステージ番号	
	StageNumberManager::stageNum_ = 0;

	// ゲームシーンフラグ
	gotoGameScene_ = false;
	// タイトルシーン
	gotoTitleScene_ = false;

	// ステージ写真
	stagePhot_ = std::make_unique<StagePhot>();
	stagePhot_->Initialize(stagePhotTextureHandles);
	// UI
	stageSelectUI_ = std::make_unique<StageSelectUI>();
	stageSelectUI_->Initialize(stageUITextureHandles);

	// セッティング
	stageSelectUI_->SetStageNum();
	stagePhot_->Setting();
}

void SelectSystem::Update()
{

	// 右移動
	if (isMoveRight_) {
		MoveRight();
	}
	// 左移動
	else if (isMoveLeft_) {
		MoveLeft();
	}
	// 入力受付
	else {

		// スティック入力
		Vector2 leftStick = input_->GetLeftAnalogstick();
		// 仮
		//leftStick = { 0.0f,0.0f };

		// 右移動
		if (leftStick.x > 0.0f) {

			isMoveRight_ = true;
			easeTimer_ = 0.0f;
			StageNumberManager::stageNum_++;
			if (StageNumberManager::stageNum_ == StageNumberManager::kStageMax) {
				StageNumberManager::stageNum_ = 0;
			}
			stageSelectUI_->SetStageNum();

			// ステージ選択SE
			audioManager_->PlayWave(kStageSelectSE);
		}
		// 左移動
		else if (leftStick.x < 0.0f) {

			isMoveLeft_ = true;
			easeTimer_ = 0.0f;
			StageNumberManager::stageNum_--;
			if (StageNumberManager::stageNum_ == -1) {
				StageNumberManager::stageNum_ = StageNumberManager::kStageMax - 1;
			}
			stageSelectUI_->SetStageNum();

			// ステージ選択SE
			audioManager_->PlayWave(kStageSelectSE);
		}
		// ゲームシーンへ
		else if (input_->PushJoystick(JoystickButton::kJoystickButtonA)) {
			// ステージ開始SE
			audioManager_->PlayWave(kStageStartSE);
			gotoGameScene_ = true;
		}
		// タイトルへ
		if (input_->PushJoystick(JoystickButton::kJoystickButtonBACK)) {
			// タイトルへ戻るSE
			audioManager_->PlayWave(kBackTitleSE);
			gotoTitleScene_ = true;
		}
	}

	// 移動していない
	if (!isMoveLeft_ && !isMoveRight_) {
		stagePhot_->Update();
		stageSelectUI_->Update();
	}

}

void SelectSystem::Draw()
{

	stagePhot_->Draw();

	stageSelectUI_->Draw(!isMoveLeft_ && !isMoveRight_);

}

void SelectSystem::MoveRight()
{

	easeTimer_ += easeSpeed_;
	if (easeTimer_ >= 1.0f) {
		isMoveRight_ = false;
		stagePhot_->MoveRight(1.0f);
	}
	else {
		stagePhot_->MoveRight(easeTimer_);
	}

}

void SelectSystem::MoveLeft()
{

	easeTimer_ += easeSpeed_;
	if (easeTimer_ >= 1.0f) {
		isMoveLeft_ = false;
		stagePhot_->MoveLeft(1.0f);
	}
	else {
		stagePhot_->MoveLeft(easeTimer_);
	}

}
