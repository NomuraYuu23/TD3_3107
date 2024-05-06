#include "CorrectSystem.h"
#include "../../Player.h"
#include "../../../../../Engine/Math/Matrix3x3.h"
#include "../../../Engine/Input/Input.h"
#include "../../../Engine/GlobalVariables/GlobalVariables.h"

void CorrectSystem::Initialize(Player* player)
{
	// ポインタ登録
	player_ = player;

}

void CorrectSystem::Update(EnemyManager* enemyManager)
{
	Input* input = Input::GetInstance();

	Vector2 rightStick = input->GetRightAnalogstick();
	Vector2 leftStick = input->GetLeftAnalogstick();
	Vector3 nearDirect = NearEnemyLockOn(enemyManager);

	//NearLockOn(enemyManager);

	if (rightStick.x != 0 || rightStick.y != 0) {
		Vector2 normalize = { rightStick.x / SHRT_MAX,rightStick.y / SHRT_MAX };

		if (std::fabsf(normalize.x) >= 0.3f || std::fabsf(normalize.y) >= 0.3f) {
			// 投げる方向ベクトル
			player_->throwDirect_ = Vector3::Normalize({ rightStick.x,rightStick.y * -1.0f,0 });

		}

		//if (isLock_) {
		//	// 対象あり
		//	// 方向確認用の行列(左範囲)
		//	float rotateWidth = GlobalVariables::GetInstance()->GetFloatValue("Player", "RotateWidth");
		//	//float rotateWidth = kRotationWidth_;
		//	Matrix3x3 leftRotateMatrix = Matrix3x3::MakeRotateMatrix(-rotateWidth);
		//	// 方向確認用の行列(右範囲)
		//	Matrix3x3 rightRotateMatrix = Matrix3x3::MakeRotateMatrix(rotateWidth);

		//	Vector2 tVec = { targetDirect_.x,targetDirect_.y };
		//	rightStick = { rightStick.x / SHRT_MAX,rightStick.y / SHRT_MAX };
		//	rightStick.y *= -1.0f;
		//	// クロス積左
		//	float leftCross = Vector2::Cross(tVec, Matrix3x3::Transform(rightStick, leftRotateMatrix));
		//	// クロス積右
		//	float rightCross = Vector2::Cross(tVec, Matrix3x3::Transform(rightStick, rightRotateMatrix));		
		//	// 方向確認
		//	if (leftCross * rightCross <= 0.0f) {
		//		player_->throwDirect_ = { tVec.x,tVec.y,0 };
		//	}
		//	else {
		//		player_->throwDirect_ = { rightStick.x,rightStick.y,0 };
		//	}

		//}
		//else {
		//	// 対象なし
		//	Vector2 normalize = { rightStick.x / SHRT_MAX,rightStick.y / SHRT_MAX };
		//	normalize.y *= -1.0f;
		//	player_->throwDirect_ = { normalize.x,normalize.y,0 };
		//}
	}
	else if (isInNearArea_ && std::holds_alternative<HoldState*>(player_->GetWeapon()->GetNowState())) {
		//// プレイヤーの向きとターゲット対象の向きが不一致の場合
		//if (targetDirect_.x > 0 && player_->isLeft_) {
		//	// Y軸補正の判断
		//	if (targetDirect_.y > 0) {
		//		player_->throwDirect_ = { -0.5f,0.5f };
		//	}
		//	else if (targetDirect_.y < 0) {
		//		player_->throwDirect_ = { -0.5f,-0.5f };
		//	}
		//	else {
		//		player_->throwDirect_ = { -0.5f,0.0f };
		//	}

		//	isLock_ = false;
		//	return;
		//}
		//// プレイヤーの向きとターゲット対象の向きが不一致の場合
		//else if (targetDirect_.x < 0 && !player_->isLeft_) {
		//	// Y軸補正の判断
		//	if (targetDirect_.y > 0) {
		//		player_->throwDirect_ = { 0.5f,0.5f };
		//	}
		//	else if (targetDirect_.y < 0) {
		//		player_->throwDirect_ = { 0.5f,-0.5f };
		//	}
		//	else {
		//		player_->throwDirect_ = { 0.5f,0.0f };
		//	}

		//	isLock_ = false;
		//	return;
		//}
		//player_->throwDirect_ = targetDirect_;
		// プレイヤーの向きとターゲット対象の向きが不一致の場合
		if (nearDirect.x > 0 && player_->isLeft_) {
			// Y軸補正の判断
			if (nearDirect.y > 0) {
				player_->throwDirect_ = { -0.5f,0.5f };
			}
			else if (nearDirect.y < 0) {
				player_->throwDirect_ = { -0.5f,-0.5f };
			}
			else {
				player_->throwDirect_ = { -0.5f,0.0f };
			}
			return;
		}
		// プレイヤーの向きとターゲット対象の向きが不一致の場合
		else if (nearDirect.x < 0 && !player_->isLeft_) {
			// Y軸補正の判断
			if (nearDirect.y > 0) {
				player_->throwDirect_ = { 0.5f,0.5f };
			}
			else if (nearDirect.y < 0) {
				player_->throwDirect_ = { 0.5f,-0.5f };
			}
			else {
				player_->throwDirect_ = { 0.5f,0.0f };
			}
			return;
		}
		player_->throwDirect_ = nearDirect;
	}
	else if (leftStick.x != 0 || leftStick.y != 0) {
		Vector2 normalLeft = { leftStick.x / SHRT_MAX,leftStick.y / SHRT_MAX };

		player_->throwDirect_.x = normalLeft.x;

		isLock_ = false;
	}

	prevLeftStick_ = leftStick;
	// 方向ベクトル
	//targetDirection;
	//player_->throwDirect_;
	isInNearArea_ = false;
}

void CorrectSystem::ImGuiDraw()
{
	ImGui::Text("IsLOck : %d", isLock_);
	ImGui::Text("isInNearArea : %d", isInNearArea_);
	ImGui::DragFloat3("TVect", &targetDirect_.x);
}

void CorrectSystem::NearLockOn(EnemyManager* enemyManager)
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
	float lengthMin = GlobalVariables::GetInstance()->GetFloatValue("Player", "InitLength");
	//float lengthMin = kInitLengthMin_;
	// 方向ベクトル
	Vector3 direction = { 0.0f, 0.0f,0.0f };
	// 目指す方向ベクトル
	Vector3 targetDirection = player_->throwDirect_;

	// 方向確認用のベクトル(プレイヤー)
	Vector2 playerDirection = { player_->throwDirect_.x, player_->throwDirect_.y };
	// 方向確認用の行列(左範囲)
	float rotateWidth = GlobalVariables::GetInstance()->GetFloatValue("Player", "RotateWidth");
	//float rotateWidth = kRotationWidth_;
	Matrix3x3 leftRotateMatrix = Matrix3x3::MakeRotateMatrix(-rotateWidth);
	// 方向確認用の行列(右範囲)
	Matrix3x3 rightRotateMatrix = Matrix3x3::MakeRotateMatrix(rotateWidth);
	// 方向確認用のベクトル(エネミー)
	Vector2 enemyDirection = { 0.0f, 0.0f };

	// クロス積左
	float leftCross = 0.0f;
	// クロス積右
	float rightCross = 0.0f;

	if (isLock_) {
		return;
	}

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

				isInNearArea_ = true;
				// 目指す方向ベクトルを更新
				targetDirect_ = direction;
				// 現在の最小の長さを更新
				lengthMin = length;

				isLock_ = true;
			}
		}
	}
}

Vector3 CorrectSystem::NearEnemyLockOn(EnemyManager* enemyManager)
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
	float lengthMin = GlobalVariables::GetInstance()->GetFloatValue("Player", "InitLength");
	//float lengthMin = kInitLengthMin_;
	// 方向ベクトル
	Vector3 direction = { 0.0f, 0.0f,0.0f };
	// 目指す方向ベクトル
	Vector3 targetDirection = player_->throwDirect_;

	// 方向確認用のベクトル(プレイヤー)
	Vector2 playerDirection = { player_->throwDirect_.x, player_->throwDirect_.y };
	// 方向確認用の行列(左範囲)
	float rotateWidth = GlobalVariables::GetInstance()->GetFloatValue("Player", "RotateWidth");
	//float rotateWidth = kRotationWidth_;
	Matrix3x3 leftRotateMatrix = Matrix3x3::MakeRotateMatrix(-rotateWidth);
	// 方向確認用の行列(右範囲)
	Matrix3x3 rightRotateMatrix = Matrix3x3::MakeRotateMatrix(rotateWidth);
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

				isInNearArea_ = true;
				// 目指す方向ベクトルを更新
				targetDirection = direction;
				// 現在の最小の長さを更新
				lengthMin = length;

			}

		}

	}
	return targetDirection;
}
