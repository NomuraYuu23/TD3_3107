#include "PlayerAnimManager.h"
#include "../Player.h"

void PlayerAnimManager::Init(Player* player)
{
	// 入力取得
	input_ = Input::GetInstance();

	// プレイヤー取得
	player_ = player;

	// アニメーション取得と初期化
	anim_.Initialize(
		player_->model_->GetNodeAnimationData(),
		player_->localMatrixManager_->GetInitTransform(),
		player_->localMatrixManager_->GetNodeNames());

	// アニメーション開始
	anim_.StartAnimation(0, true);

	// 角度の初期設定
	player_->worldtransform_.transform_.rotate.y = (static_cast<float>(std::numbers::pi) / 2.0f) * -1.0f;
}

void PlayerAnimManager::Update()
{
	// プレイヤーをスティックの入力方向に向かせる
	if (player_->velocity_.x > 0.0f || player_->velocity_.x < 0.0f) {
		// 左スティックの入力取得
		Vector2 leftStick = input_->GetLeftAnalogstick();
		// 入力正規化
		leftStick = Vector2::Normalize(leftStick);

		// 入力の向きによってプレイヤーの向きを変える
		if (leftStick.x > 0.0f) {
			// 右に向かせる
			isRight_ = true;
			player_->worldtransform_.transform_.rotate.y = (static_cast<float>(std::numbers::pi) / 2.0f) * -1.0f;
		}
		else {
			// 左に向かせる
			isRight_ = false;
			player_->worldtransform_.transform_.rotate.y = (static_cast<float>(std::numbers::pi) / 2.0f);
		}

		// 走りアニメーションが再生されていない場合
		if (!anim_.GetRunningAnimation(Run) && !anim_.GetRunningAnimation(JumpStart) && !anim_.GetRunningAnimation(SpearJump) && player_->isGround_) {
			// 全アニメーション停止
			StopAnimationAll();
			// アニメーション再生
			anim_.StartAnimation(Run, true);
		}
	}
	else if(player_->isGround_ && !anim_.GetRunningAnimation(JumpStart) && !anim_.GetRunningAnimation(SpearJump) && !anim_.GetRunningAnimation(Landing)){
		// 待機アニメーションが再生されていない場合
		if (!anim_.GetRunningAnimation(Idle)) {
			// 全アニメーション停止
			StopAnimationAll();
			// アニメーション再生
			anim_.StartAnimation(Idle, true);
		}
	}

	// 落下中アニメーションが再生されていない場合
	if (!anim_.GetRunningAnimation(Jumping) && !anim_.GetRunningAnimation(JumpStart) && !anim_.GetRunningAnimation(SpearJump) && !player_->isGround_) {
		// 全アニメーション停止
		StopAnimationAll();
		// アニメーション再生
		anim_.StartAnimation(Jumping, true);
	}

	// アニメーションの更新
	player_->localMatrixManager_->SetNodeLocalMatrix(anim_.AnimationUpdate());
	player_->localMatrixManager_->Map();
}

void PlayerAnimManager::PlayAnimation(int32_t animNum, bool isLoop, bool isFinish)
{
	// アニメーション停止トリガーで分岐
	if (isFinish) {
		// 全アニメーション停止
		StopAnimationAll();
	}

	// 指定した番号のアニメーション再生
	anim_.StartAnimation(animNum, isLoop);
}

void PlayerAnimManager::StopAnimationAll()
{
	// 全アニメーション分ループ
	for (int i = 0; i < SpearJump; i++) {
		anim_.StopAnimation(i);
	}
}
