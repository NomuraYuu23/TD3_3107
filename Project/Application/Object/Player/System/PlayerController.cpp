#include "PlayerController.h"
#include "../Player.h"
#include "../../../Engine/base/TextureManager.h"
#include "../../../Engine/GlobalVariables/GlobalVariables.h"

void PlayerController::Initialize(Player* player)
{

	input_ = Input::GetInstance();

	player_ = player;

}

void PlayerController::Update()
{
	//// 入力キャンセル
	//if (player_->IsRecoil()) {
	//	return;
	//}

	// コントローラー用
	ControllerProcess();

#ifdef _DEBUG
	if (input_->TriggerKey(DIK_H)) {
		player_->ChangeState(std::make_unique<AerialState>());
	}

	//// 移動入力
	//if (input_->PushKey(DIK_A)) {
	//	player_->velocity_.x = -5.f;
	//}
	//else if (input_->PushKey(DIK_D)) {
	//	player_->velocity_.x = 5.f;
	//}
	//else {
	//	player_->velocity_.x = 0;
	//}


#endif // _DEBUG


	// キーボード用
	//KeyBoardProcess();


}

void PlayerController::ControllerProcess()
{
	//Vector2 leftStick = input_->GetLeftAnalogstick();
	//float moveSpeed_ = 15.0f;
	bool CheckAction = false;
	player_->isArrowUiDraw_ = false;

	if (input_->GetJoystickConnected()) {
		// 地上処理
		GroundMoveProcess();

		// 空中処理
		AerialMoveProcess();

		//---どの状態でも行える操作---//

		// 投げ
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

		// スローモーション
		if (std::holds_alternative<HoldState*>(player_->weapon_->nowState_)) {
			if (stickDirect.x == 0 && stickDirect.y == 0) {
				player_->sPlaySpeed = 1.0f;
			}
			else {
				// スローの倍率
				player_->sPlaySpeed = GlobalVariables::GetInstance()->GetFloatValue("Common", "SlowFactor");
				// UI表示
				player_->isArrowUiDraw_ = true;
			}
		}
		else {
			player_->sPlaySpeed = 1.0f;
		}
		player_->throwDirect_ = Vector3::Normalize({ stickDirect.x,stickDirect.y * -1.0f,0 });

	}

	player_->worldtransform_.transform_.translate.x += player_->velocity_.x * kDeltaTime_ * (1.0f / IObject::sPlaySpeed);

}

void PlayerController::AerialMoveProcess()
{
	// 入力キャンセル
	if (player_->IsRecoil()) {
		return;
	}

	Vector2 leftStick = input_->GetLeftAnalogstick();
	float moveSpeed_ = GlobalVariables::GetInstance()->GetFloatValue("Player", "AerialAcceleration");
	bool CheckAction = std::holds_alternative<AerialState*>(player_->GetNowState()) || std::holds_alternative<SpearAerialState*>(player_->GetNowState());

	// 地上にいる場合
	if (CheckAction) {
		// 左右移動
		player_->velocity_.x += (float)leftStick.x / SHRT_MAX * moveSpeed_ * kDeltaTime_ * (1.0f / IObject::sPlaySpeed);

	}
}

void PlayerController::GroundMoveProcess()
{
	// 入力キャンセル
	if (player_->IsRecoil()) {
		return;
	}

	Vector2 leftStick = input_->GetLeftAnalogstick();
	float moveSpeed_ = GlobalVariables::GetInstance()->GetFloatValue("Player", "MoveSpeed");
	bool CheckAction = std::holds_alternative<GroundState*>(player_->GetNowState());

	// 地上にいる場合
	if (CheckAction) {
		// 左右移動
		player_->velocity_.x = (float)leftStick.x / SHRT_MAX * moveSpeed_ * (1.0f / IObject::sPlaySpeed);

		//player_->velocity_.x += (float)leftStick.x / SHRT_MAX * moveSpeed_ * kDeltaTime_;

		//if (player_->velocity_.x >= 10.0f) {
		//	player_->velocity_.x = 10.0f;
		//}
		//else if (player_->velocity_.x <= -10.0f) {
		//	player_->velocity_.x = -10.0f;
		//}

		// ジャンプ
		// ジャンプ中は入力を受け付けない
		if (input_->TriggerJoystick(kJoystickButtonA)) {
			// 切り替え
			player_->ChangeState(std::make_unique<AerialState>());
			return;
		}
	}
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
