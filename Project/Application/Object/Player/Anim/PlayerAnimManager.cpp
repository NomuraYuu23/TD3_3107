#include "PlayerAnimManager.h"
#include "../Player.h"

void PlayerAnimManager::Init(Player* player)
{
	// 入力取得
	input_ = Input::GetInstance();

	// プレイヤー取得
	player_ = player;

	// アニメーション取得と初期化
	spearAnim_.Initialize(
		player_->model_->GetNodeAnimationData(),
		player_->localMatrixManager_->GetInitTransform(),
		player_->localMatrixManager_->GetNodeNames());

	// アニメーション開始
	spearAnim_.StartAnimation(0, true);

	// 角度の初期設定
	player_->worldtransform_.transform_.rotate.y = (static_cast<float>(std::numbers::pi) / 2.0f) * -1.0f;

	// アニメーションの更新
	player_->localMatrixManager_->SetNodeLocalMatrix(spearAnim_.AnimationUpdate());
	player_->localMatrixManager_->Map();
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
		if (!spearAnim_.GetRunningAnimation(Run) && !spearAnim_.GetRunningAnimation(JumpStart) && !spearAnim_.GetRunningAnimation(SpearJump) && player_->isGround_) {
			// 全アニメーション停止
			StopSpearAnimationAll();
			// アニメーション再生
			spearAnim_.StartAnimation(Run, true);
		}

		EulerTransform transform = player_->worldtransform_.transform_;
		transform.translate.y -= 2.0f;

		if (player_->isGround_) {
			// 走りパーティクル
			EmitterDesc desc;
			desc.transform = &transform;
			desc.instanceCount = 1;
			desc.frequency = 0.01f;
			desc.lifeTime = 0.01f;
			desc.particleModelNum = kCircle;
			desc.paeticleName = kRunSmokeParticle;

			ParticleManager::GetInstance()->MakeEmitter(&desc, 0);
		}
		
	}
	else if(player_->isGround_ && !spearAnim_.GetRunningAnimation(JumpStart) && !spearAnim_.GetRunningAnimation(SpearJump) && !spearAnim_.GetRunningAnimation(Landing)){
		// 待機アニメーションが再生されていない場合
		if (!spearAnim_.GetRunningAnimation(Idle)) {
			// 全アニメーション停止
			StopSpearAnimationAll();
			// アニメーション再生
			spearAnim_.StartAnimation(Idle, true);
		}
	}

	// 落下中アニメーションが再生されていない場合
	if (!spearAnim_.GetRunningAnimation(Jumping) && !spearAnim_.GetRunningAnimation(JumpStart) && !spearAnim_.GetRunningAnimation(SpearJump) && !player_->isGround_) {
		// 全アニメーション停止
		StopSpearAnimationAll();
		// アニメーション再生
		spearAnim_.StartAnimation(Jumping, true);
	}

	// アニメーションの更新
	player_->localMatrixManager_->SetNodeLocalMatrix(spearAnim_.AnimationUpdate());
	player_->localMatrixManager_->Map();
}

void PlayerAnimManager::PlaySpearAnimation(int32_t animNum, bool isLoop, bool isFinish)
{
	// アニメーション停止トリガーで分岐
	if (isFinish) {
		// 全アニメーション停止
		StopSpearAnimationAll();
	}

	// 指定した番号のアニメーション再生
	spearAnim_.StartAnimation(animNum, isLoop);
}

void PlayerAnimManager::StopSpearAnimationAll()
{
	// 全アニメーション分ループ
	for (int i = 0; i < PlayerAnimCount; i++) {
		spearAnim_.StopAnimation(i);
	}
}
