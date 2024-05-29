#include "DetectionSystem.h"
#include "../../ObjectList.h"

void DetectionSystem::Initialize(Enemy* enemy)
{
	// それぞれのポインタを渡す
	enemy_ = enemy;

}

void DetectionSystem::Update(bool isRayCollision)
{
	if (std::holds_alternative<ChaseEnemyState*>(enemy_->GetState())) {
		// 壁が間にない場合
		if (!isRayCollision) {
			// 追尾フラグ設定
			isChase_ = ShouldChase(enemy_->GetPlayer()->worldtransform_.GetWorldPosition(), searchLength_);
		}
		else {
			// 追尾キャンセル
			isChase_ = false;
		}
	}

	// 追尾可能状態の処理
	if (isChase_) {
		ChaseUpdate();
	}
}

bool DetectionSystem::ShouldChase(const Vector3& target, float searchLength)
{
	// それぞれの座標
	Vector3 playerPosition = target;
	Vector3 enemyPosition = enemy_->GetWorldPosition();
	// 距離の長さ
	float length = Vector3::Length(playerPosition - enemyPosition);

	// 範囲内かチェック
	if (length < searchLength) {
		return true;
	}

	return false;
}

void DetectionSystem::ChaseUpdate()
{
	// 移動方向ベクトル
	Vector3 moveDirect = enemy_->GetPlayer()->worldtransform_.GetWorldPosition() - enemy_->GetWorldPosition();
	// 方向
	enemy_->direction_ = { moveDirect.x,moveDirect.y , 0};
	// 正規化
	moveDirect = Vector3::Normalize(moveDirect);

	// 移動速度
	float moveRate = GlobalVariables::GetInstance()->GetFloatValue("Enemy", "ChaseSpeed");

	// 追尾計算（今後変更予定
	enemy_->transform_.translate.x += moveDirect.x * (kDeltaTime_) * (1.0f / GameSystemManager::sGameSpeed) * moveRate;
	enemy_->transform_.translate.y += moveDirect.y * (kDeltaTime_) * (1.0f / GameSystemManager::sGameSpeed) * moveRate;


	enemy_->MatrixUpdate();
}
