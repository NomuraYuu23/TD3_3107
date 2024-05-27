#include "SingleEnemyState.h"
#include "../Enemy.h"
#include "../../../Engine/GlobalVariables/GlobalVariables.h"
#include "../../GameSystem/GameSystemManager.h"

void SingleEnemyState::Initialize()
{
	enemy_->SetState(this);
	enemy_->MatrixUpdate();
	// ※エディターに入れるタイミングで消せ
	actionPattern_ = static_cast<uint32_t>(IEnemyState::ActionMode::kPatrolMode);
	// 行動の初期化
	(this->*actionFuncs_[actionPattern_].initFunc_)();

}

void SingleEnemyState::Update()
{

	// 行動の更新
	(this->*actionFuncs_[actionPattern_].updateFunc_)();

}

void SingleEnemyState::SettingMoveInfo(const Vector3& endPoint, Ease::EaseName name)
{
	// イージングする座標
	startPoint_ = { enemy_->GetWorldPosition(),true };
	endPoint_ = { endPoint,false };
	// イージングパターン
	easeName_ = name;

}

void SingleEnemyState::PatrolInitialize()
{
	// タイマースタート
	//if (!moveChangeTimer_.IsActive()) {
	// 終了フレーム
	moveChangeFrame_ = GlobalVariables::GetInstance()->GetFloatValue("Enemy", "EaseEndFrame");
	moveChangeTimer_.Start(moveChangeFrame_);
	//}
}

void SingleEnemyState::PatrolUpdate()
{
	// 移動時間計算
	moveChangeTimer_.Update(GameSystemManager::sGameSpeed);
	// アクティブ中の移動処理
	if (moveChangeTimer_.IsActive()) {
		if (startPoint_.isStart) {
			enemy_->transform_.translate = Ease::Easing(easeName_, startPoint_.position, endPoint_.position, moveChangeTimer_.GetNowFrame());
		}
		else if (endPoint_.isStart) {
			enemy_->transform_.translate = Ease::Easing(easeName_, endPoint_.position, startPoint_.position, moveChangeTimer_.GetNowFrame());
		}
	}
	// 終了時の切り替え
	if (moveChangeTimer_.IsEnd()) {
		if (startPoint_.isStart) {
			startPoint_.isStart = false;
			endPoint_.isStart = true;
		}
		else if (endPoint_.isStart) {
			startPoint_.isStart = true;
			endPoint_.isStart = false;
		}
		moveChangeTimer_.Start(moveChangeFrame_);
	}

	// どちらを始点としているかに応じて変更
	if (startPoint_.isStart) {
		Vector3 normalize = Vector3::Normalize(endPoint_.position - startPoint_.position);
		// 向き
		if (normalize.x > 0) {
			enemy_->transform_.rotate.z = 0;
		}
		else {
			enemy_->transform_.rotate.z = 3.14f;
		}
	}
	else if (endPoint_.isStart) {
		Vector3 normalize = Vector3::Normalize(startPoint_.position - endPoint_.position);
		// 向き
		if (normalize.x > 0) {
			enemy_->transform_.rotate.z = 0;
		}
		else {
			enemy_->transform_.rotate.z = 3.14f;
		}
	}

}

void SingleEnemyState::ChaseInitialize()
{

}

void SingleEnemyState::ChaseUpdate()
{
}
