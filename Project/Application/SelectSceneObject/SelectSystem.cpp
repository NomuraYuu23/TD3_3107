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
	std::unique_ptr<StageSelectUI> stageSelectUI_ = nullptr;

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
}

void SelectSystem::MoveRight()
{
}

void SelectSystem::MoveLeft()
{
}
