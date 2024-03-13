#include "PlayerController.h"
#include "../Player.h"
#include "../../../Engine/base/TextureManager.h"

void PlayerController::Initialize(Player* player)
{

	input_ = Input::GetInstance();

	player_ = player;

}

void PlayerController::Update()
{

	if (player_->IsRecoil()) {
		return;
	}

	// コントローラー用
	ControllerProcess();

	// キーボード用
	//KeyBoardProcess();


}

void PlayerController::ControllerProcess()
{
	Vector2 leftStick = input_->GetLeftAnalogstick();
	float moveSpeed_ = 15.0f;
	bool CheckAction = false;

	if (input_->GetJoystickConnected()) {
		// 左右移動
		player_->velocity_.x = (float)leftStick.x / SHRT_MAX * moveSpeed_;
		// ジャンプ
		// ジャンプ中は入力を受け付けない
		CheckAction = std::holds_alternative<AerialState*>(player_->GetNowState());
		if (input_->TriggerJoystick(kJoystickButtonA) && !CheckAction) {
			// 切り替え
			player_->ChangeState(std::make_unique<AerialState>());
			return;
		}

		//---どの状態でも行える操作---//

		// 投げ
		//CheckAction = std::holds_alternative<HoldState*>(player_->weapon_->nowState_);
		// 武器持ってなければfalse
		if (input_->TriggerJoystick(kJoystickButtonRB)) {
			// 投げ入力
			if (std::holds_alternative<HoldState*>(player_->weapon_->nowState_)) {
				// 右スティックの入力がなければキャンセル
				if (player_->throwDirect_.x == 0.0f && player_->throwDirect_.y == 0.0f) {
					return;
				}
				// 方向
				player_->weapon_->throwDirect_ = player_->throwDirect_;
				player_->weapon_->ChangeRequest(Weapon::StateName::kThrown);
				// 地上で投げた場合は槍の重力フラグをオン
				if (std::holds_alternative<GroundState*>(player_->GetNowState())) {
					player_->weapon_->GravityInitialize();
				}
			}
			// 待機に入る
			else if (std::holds_alternative<ImpaledState*>(player_->weapon_->nowState_)) {
				player_->weapon_->ChangeRequest(Weapon::StateName::kWait);
			}
			// 戻ってくる
			else if (std::holds_alternative<ReturnWaitState*>(player_->weapon_->nowState_)) {
				player_->weapon_->ChangeRequest(Weapon::StateName::kReturn);
			}

		}
		// 戻ってくる入力
		if (input_->TriggerJoystick(kJoystickButtonLB)) {
			player_->weapon_->ChangeRequest(Weapon::StateName::kReturn);
		}

		// 投げる方向
		Vector2 stickDirect = input_->GetRightAnalogstick();
		player_->throwDirect_ = Vector3::Normalize({ stickDirect.x,stickDirect.y * -1.0f,0 });

	}

	player_->worldtransform_.transform_.translate.x += player_->velocity_.x * kDeltaTime_ * (1.0f / IObject::sPlaySpeed);

}

void PlayerController::KeyBoardProcess()
{
	// 武器
	if (player_->weapon_) {
		if (input_->TriggerKey(DIK_E)) {
			//player_->weapon_->ChangeRequest(Weapon::StateName::kReturn);
			// 待機に入る
			if (std::holds_alternative<ImpaledState*>(player_->weapon_->nowState_)) {
				player_->weapon_->ChangeRequest(Weapon::StateName::kWait);
			}
			// 戻ってくる
			else if (std::holds_alternative<ReturnWaitState*>(player_->weapon_->nowState_)) {
				player_->weapon_->ChangeRequest(Weapon::StateName::kReturn);
			}
		}
		if (input_->TriggerKey(DIK_Q)) {
			player_->weapon_->throwDirect_ = player_->throwDirect_;
			player_->weapon_->ChangeRequest(Weapon::StateName::kThrown);
		}
	}

	float moveSpeed_ = 6.0f;
	// ステート変更
	bool CheckAction = std::holds_alternative<AerialState*>(player_->GetNowState());
	if (input_->TriggerKey(DIK_SPACE) && !CheckAction) {
		// 切り替え
		player_->ChangeState(std::make_unique<AerialState>());
		return;
	}

	// 移動入力
	if (input_->PushKey(DIK_A)) {
		player_->velocity_.x = -moveSpeed_;
	}
	else if (input_->PushKey(DIK_D)) {
		player_->velocity_.x = moveSpeed_;
	}
	player_->worldtransform_.transform_.translate.x += player_->velocity_.x * kDeltaTime_;

}
