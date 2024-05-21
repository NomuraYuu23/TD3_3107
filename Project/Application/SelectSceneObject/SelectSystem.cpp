#include "SelectSystem.h"

void SelectSystem::Initialize(
	const std::array<uint32_t, kStageMax>& stagePhotTextureHandles,
	const std::array<uint32_t, StageSelectUI::SpriteIndex::kSpriteIndexOfCount>& stageUITextureHandles)
{

	// 入力
	input_ = Input::GetInstance();

	// 動いているか
	isMoveRight_ = false;
	isMoveLeft_ = false;

	// イージング
	easeTimer_ = 0.0f;
	easeSpeed_ = 0.01f;

	// ステージ番号	
	stageNum_ = 0;

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
	stageSelectUI_->SetStageNum(stageNum_);
	stagePhot_->Setting(stageNum_);

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


		// 右移動
		if (leftStick.x > 0.0f) {

			isMoveRight_ = true;
			easeTimer_ = 0.0f;
			stageNum_++;
			if (stageNum_ == kStageMax) {
				stageNum_ = 0;
			}
			stageSelectUI_->SetStageNum(stageNum_);
		}
		// 左移動
		else if (leftStick.x < 0.0f) {

			isMoveLeft_ = true;
			easeTimer_ = 0.0f;
			stageNum_--;
			if (stageNum_ == -1) {
				stageNum_ = kStageMax - 1;
			}
			stageSelectUI_->SetStageNum(stageNum_);
		}
		// ゲームシーンへ
		else if (input_->PushJoystick(JoystickButton::kJoystickButtonA)) {
			gotoGameScene_ = true;
		}
		// タイトルへ
		if (input_->PushJoystick(JoystickButton::kJoystickButtonBACK)) {
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
