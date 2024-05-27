#include "PlayerController.h"
#include "../Player.h"
#include "../../../Engine/base/TextureManager.h"
#include "../../../Engine/GlobalVariables/GlobalVariables.h"

void PlayerController::Initialize(Player* player)
{

	input_ = Input::GetInstance();

	player_ = player;
	groundSpeed_ = GlobalVariables::GetInstance()->GetFloatValue("Player", "MoveSpeed");
	aerialSpeed_ = GlobalVariables::GetInstance()->GetFloatValue("SpearJump", "AerialAcceleration");
}

void PlayerController::Update()
{
	//// 入力キャンセル
	//if (player_->IsRecoil()) {
	//	return;
	//}

	// コントローラー用
	ControllerProcess();
	if (player_->velocity_.x > 0) {
		player_->isLeft_ = false;
	}
	else if (player_->velocity_.x < 0) {
		player_->isLeft_ = true;
	}
#ifdef _DEBUG
	if (input_->TriggerKey(DIK_H)) {
		player_->ChangeState(std::make_unique<AerialState>());
	}
	groundSpeed_ = GlobalVariables::GetInstance()->GetFloatValue("Player", "MoveSpeed");
	aerialSpeed_ = GlobalVariables::GetInstance()->GetFloatValue("SpearJump", "AerialAcceleration");

#endif // _DEBUG


	// キーボード用
	//KeyBoardProcess();


}

void PlayerController::ImGuiDraw()
{
	// 投げる方向
	Vector2 stickDirect = input_->GetRightAnalogstick();
	ImGui::DragFloat2("rightStick", &stickDirect.x);
	Vector2 normalize = { stickDirect.x / SHRT_MAX,stickDirect.y / SHRT_MAX };
	ImGui::DragFloat2("normStick", &normalize.x);
}

void PlayerController::ControllerProcess()
{
	//Vector2 leftStick = input_->GetLeftAnalogstick();
	//float moveSpeed_ = 15.0f;
	bool CheckAction = false;
	player_->isArrowUiDraw_ = false;

	if (input_->GetJoystickConnected()) {
		// 待機処理
		WaitKeyProcess();

		// 空中処理
		AerialMoveProcess();

		// 地上処理
		GroundMoveProcess();

		//---どの状態でも行える操作---//

		// 投げ処理
		ThrownProcess();

		// 戻ってくる入力
		//if (input_->TriggerJoystick(kJoystickButtonLB) && std::holds_alternative<ImpaledState*>(player_->weapon_->GetNowState())) {
		//	player_->weapon_->ChangeRequest(Weapon::StateName::kReturn);
		//}

		if (player_->IsRecoil()) {
			player_->isArrowUiDraw_ = true;
			return;
		}

		Vector2 leftStick = input_->GetLeftAnalogstick();
		leftStick = { leftStick.x / SHRT_MAX,leftStick.y / SHRT_MAX * -1.0f };

		if (std::fabsf(leftStick.x) > 0.25f) {
			player_->worldtransform_.direction_.x = leftStick.x;
		}
		if (std::fabsf(leftStick.y) > 0.25f) {
			player_->worldtransform_.direction_.y = leftStick.y;
		}

		// 投げる方向
		Vector2 stickDirect = input_->GetRightAnalogstick();
		player_->isSlowNow_ = false;

		// スローモーション
		if (std::holds_alternative<HoldState*>(player_->weapon_->GetNowState())) {
			// スロー処理
			Vector2 deadZone = { stickDirect.x / SHRT_MAX,stickDirect.y / SHRT_MAX };
			float deadZoneValue = 0.25f;
			if ((std::fabsf(stickDirect.x) > deadZoneValue || std::fabsf(stickDirect.y) > deadZoneValue) &&
				!player_->IsRecoil()) {
				if (!std::holds_alternative<GroundState*>(player_->GetNowState())) {
					// UI表示
					player_->isArrowUiDraw_ = true;
					player_->isSlowNow_ = true;
				}
			}

			// デバッグ以外の場合行う
			#ifndef _DEBUG
			// 槍のエイムアニメーション再生
			/*if (!player_->weapon_->GetAnimManager()->GetSpearAnim().GetRunningAnimation(SpearAnimManager::SpearAim)) {
				player_->weapon_->GetAnimManager()->PlaySpearAnimation(SpearAnimManager::SpearAim);
			}*/
			#endif // !_DEBUG

			// 槍を持っているなら槍の向きを狙っている方向に合わせるように指示
			if (player_->weapon_->isHold_) {
				player_->weapon_->throwDirect_ = player_->throwDirect_;
			}
		}
		else {
			// 槍を持っているなら槍の向きを狙っている方向に合わせるように指示
			if (player_->weapon_->isHold_) {
				player_->weapon_->throwDirect_ = { 0,1,0 };
			}

			#ifndef _DEBUG
			// 何も再生されていなければ待機アニメーション再生
			if (player_->weapon_->GetAnimManager()->GetSpearAnim().GetRunningAnimation()) {
				//player_->weapon_->GetAnimManager()->PlaySpearAnimation(SpearAnimManager::SpearIdle, true);
		}
			#endif // !_DEBUG
		}

		// スローの判定
		if (player_->isSlowNow_) {
			// スローの倍率
			GameSystemManager::sGameSpeed = GlobalVariables::GetInstance()->GetFloatValue("Common", "SlowFactor");
			//player_->sPlaySpeed = GlobalVariables::GetInstance()->GetFloatValue("Common", "SlowFactor");
		}
		else {
			GameSystemManager::sGameSpeed = 1.0f;
			//player_->sPlaySpeed = 1.0f;
		}

	}
	// 座標更新
	if (std::holds_alternative<GroundState*>(player_->GetNowState())) {
		player_->worldtransform_.transform_.translate.x += player_->velocity_.x * kDeltaTime_ * (1.0f / GameSystemManager::sGameSpeed);
	}

}

void PlayerController::AerialMoveProcess()
{
	// 入力キャンセル
	if (player_->IsRecoil()) {
		return;
	}

	Vector2 leftStick = input_->GetLeftAnalogstick();
	bool CheckAction = (std::holds_alternative<AerialState*>(player_->GetNowState()) || std::holds_alternative<SpearAerialState*>(player_->GetNowState()));
	float ratio = GlobalVariables::GetInstance()->GetFloatValue("SpearJump", "inverceRatio");
	// 空中にいる場合
	if (CheckAction) {
		// 左右移動
		if (player_->velocity_.x > 0 && (leftStick.x / SHRT_MAX) < 0) {
			player_->velocity_.x += (float)leftStick.x / SHRT_MAX * (aerialSpeed_ * ratio) * kDeltaTime_ * (1.0f / GameSystemManager::sGameSpeed);
		}
		else if (player_->velocity_.x < 0 && (leftStick.x / SHRT_MAX) > 0) {
			player_->velocity_.x += (float)leftStick.x / SHRT_MAX * (aerialSpeed_ * ratio) * kDeltaTime_ * (1.0f / GameSystemManager::sGameSpeed);
		}
		else {
			player_->velocity_.x += (float)leftStick.x / SHRT_MAX * aerialSpeed_ * kDeltaTime_ * (1.0f / GameSystemManager::sGameSpeed);
		}

	}
}

void PlayerController::GroundMoveProcess()
{
	// 入力キャンセル
	if (player_->IsRecoil()) {
		return;
	}

	Vector2 leftStick = input_->GetLeftAnalogstick();
	bool CheckAction = std::holds_alternative<GroundState*>(player_->GetNowState());

	// 地上にいる場合
	if (CheckAction) {
		// 左右移動
		float moveValue = 0;
		if (leftStick.x > 0) {
			moveValue = 1.0f;
		}
		else if (leftStick.x < 0) {
			moveValue = -1.0f;
		}
		//float moveValue = (float)leftStick.x / SHRT_MAX;
		player_->velocity_.x = moveValue * groundSpeed_ * (1.0f / GameSystemManager::sGameSpeed);

		// ジャンプ
		// ジャンプ中は入力を受け付けない
		if ((input_->TriggerJoystick(kJoystickButtonLB) || input_->TriggerJoystick(kJoystickButtonA)) && player_->isGround_) {
			// 切り替え
			player_->ChangeState(std::make_unique<AerialState>());
			return;
		}
	}
}

void PlayerController::WaitKeyProcess()
{
	Vector2 leftStick = input_->GetLeftAnalogstick();
	bool CheckAction = std::holds_alternative<ActionWaitState*>(player_->GetNowState());
	if (!CheckAction) {
		return;
	}
	// 移動ベクトルが下向きの時にのみ
	if (input_->TriggerJoystick(kJoystickButtonLB) && (!player_->IsRecoil())) {
		// 踏む際の武器設定
		player_->weapon_->TreadSetting();
		// 槍じゃんステートへ
		player_->ChangeState(std::make_unique<SpearAerialState>());
	}
	return;

}

void PlayerController::ThrownProcess()
{
	if (input_->TriggerJoystick(kJoystickButtonRB)) {
		// 投げ入力
		if (std::holds_alternative<HoldState*>(player_->weapon_->GetNowState())) {
			// 右スティックの入力がなければキャンセル
			if (player_->throwDirect_.x == 0.0f && player_->throwDirect_.y == 0.0f) {
				return;
			}
			// 地上で投げた場合は槍の重力フラグをオン
			if (std::holds_alternative<GroundState*>(player_->GetNowState())) {
				player_->weapon_->SetIsGravity(false);
			}
			else {
				player_->weapon_->SetIsGravity(false);
			}
			// 方向
			player_->weapon_->throwDirect_ = player_->throwDirect_;
			player_->weapon_->throwDirect_ = Vector3::Normalize(player_->weapon_->throwDirect_);
			player_->weapon_->ChangeRequest(Weapon::StateName::kThrown);
		}
		// 刺さってる→戻ってくる
		else if (std::holds_alternative<ImpaledState*>(player_->weapon_->GetNowState())) {
			if (!std::holds_alternative<AttractState*>(player_->GetNowState())) {
				player_->weapon_->ChangeRequest(Weapon::StateName::kReturn);
			}
		}
		else if (std::holds_alternative<FreeFallState*>(player_->weapon_->GetNowState())) {
			if (!player_->IsCanReturn()) {
				return;
			}
			if (!player_->FreeFallActive()) {
				player_->weapon_->ChangeRequest(Weapon::StateName::kReturn);
			}
		}
		// 待機→戻ってくる
		else if (std::holds_alternative<ReturnWaitState*>(player_->weapon_->GetNowState())) {
			player_->weapon_->ChangeRequest(Weapon::StateName::kReturn);
		}

	}
	//if (input_->TriggerJoystick(kJoystickButtonLB)) {
	//	if ((std::holds_alternative<AerialState*>(player_->GetNowState()) || std::holds_alternative<SpearAerialState*>(player_->GetNowState()))) {
	//		// 切り替え
	//		if (std::holds_alternative<ImpaledState*>(player_->weapon_->GetNowState())) {
	//			player_->ChangeState(std::make_unique<AttractState>());
	//			return;
	//		}
	//	}
	//}

}

void PlayerController::KeyBoardProcess()
{
	// 武器
	if (player_->weapon_) {
		if (input_->TriggerKey(DIK_E)) {
			//player_->weapon_->ChangeRequest(Weapon::StateName::kReturn);
			// 待機に入る
			if (std::holds_alternative<ImpaledState*>(player_->weapon_->GetNowState())) {
				player_->weapon_->ChangeRequest(Weapon::StateName::kWait);
			}
			// 戻ってくる
			else if (std::holds_alternative<ReturnWaitState*>(player_->weapon_->GetNowState())) {
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
