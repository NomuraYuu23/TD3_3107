#include "AssistDash.h"
#include "../../Player.h"
#include "../../../Engine/Input/Input.h"

void AssistDash::Update()
{
	// 落下減速タイマー
	fallSlowTimer_.Update();
	// 受付タイマー
	acceptTimer_.Update();
	// 入力受付中
	if (acceptTimer_.IsActive()) {
		
		float deadZone = GlobalVariables::GetInstance()->GetFloatValue("Common", "DeadZone");
		inputDirect_ = Input::GetInstance()->GetLeftAnalogstick();
		// 解除処理
		if (std::fabsf(inputDirect_.x) > deadZone) {
			// ダッシュ処理
			Dash();
			// 終了
			acceptTimer_.End();
		}
	}

}

void AssistDash::StartAccept(float acceptFrame)
{
	// 入力方向
	inputDirect_ = {};
	// 受付開始
	acceptTimer_.Start(acceptFrame);
}

void AssistDash::Dash()
{
	float dashPower = GlobalVariables::GetInstance()->GetFloatValue("Dash", "DashPower");
	if (inputDirect_.x > 0) {
		player_->velocity_.x += dashPower;
	}
	else if (inputDirect_.x < 0) {
		player_->velocity_.x -= dashPower;
	}

	// 速度補正タイマー
	float frame = GlobalVariables::GetInstance()->GetFloatValue("Dash", "SlowFrame");
	this->fallSlowTimer_.Start(frame);

}
