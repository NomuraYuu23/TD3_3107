#include "CorrectSystem.h"
#include "../../Player.h"
#include "../../../../../Engine/Math/Matrix3x3.h"

void CorrectSystem::Initialize(Player* player)
{
	// ポインタ登録
	player_ = player;

}

void CorrectSystem::Update(EnemyManager* enemyManager)
{

	// プレイヤーのワールドポジション
	Vector3 playerPos = player_->worldtransform_.GetWorldPosition();
	// エネミーのワールドポジション
	Vector3 enemyPos = { 0.0f,0.0f,0.0f };
	// エネミーへのベクトル
	Vector3 toEnemy = { 0.0f,0.0f,0.0f };
	// 長さ
	float length = 0.0f;
	// 現在の最小の長さ
	float lengthMin = kInitLengthMin_;
	// 方向ベクトル
	Vector3 direction = { 0.0f, 0.0f,0.0f };
	// 目指す方向ベクトル
	Vector3 targetDirection = player_->throwDirect_;

	// 方向確認用のベクトル(プレイヤー)
	Vector2 playerDirection = { player_->throwDirect_.x, player_->throwDirect_.y };
	// 方向確認用の行列(左範囲)
	Matrix3x3 leftRotateMatrix = Matrix3x3::MakeRotateMatrix(-kRotationWidth_);
	// 方向確認用の行列(右範囲)
	Matrix3x3 rightRotateMatrix = Matrix3x3::MakeRotateMatrix(kRotationWidth_);
	// 方向確認用のベクトル(エネミー)
	Vector2 enemyDirection = { 0.0f, 0.0f };

	// クロス積左
	float leftCross = 0.0f;
	// クロス積右
	float rightCross = 0.0f;

	// ループ文
	std::list<std::unique_ptr<OneOfManyObjects>>::iterator itr = enemyManager->GetObjects()->begin();
	for (; itr != enemyManager->GetObjects()->end(); ++itr) {
		
		// エネミーをとってくる
		OneOfManyObjects* obj = itr->get();
		// エネミーのポジションをとる
		enemyPos = obj->GetWorldPosition();
		// エネミーへのベクトルを作成
		toEnemy = enemyPos - playerPos;
		// 長さを作成
		length = Vector3::Length(toEnemy);
		// 長さが短いか確認
		if (length < lengthMin) {

			// 方向ベクトルを作成
			direction = Vector3::Normalize(toEnemy);
			enemyDirection = { direction.x, direction.y };

			// 方向確認
			leftCross = Vector2::Cross(enemyDirection, Matrix3x3::Transform(playerDirection, leftRotateMatrix));
			rightCross = Vector2::Cross(enemyDirection, Matrix3x3::Transform(playerDirection, rightRotateMatrix));

			if (leftCross * rightCross <= 0.0f) {

				// 目指す方向ベクトルを更新
				targetDirection = direction;
				// 現在の最小の長さを更新
				lengthMin = length;

			}

		}

	}


	player_->throwDirect_ = targetDirection;
	// 方向ベクトル
	//targetDirection;

}
