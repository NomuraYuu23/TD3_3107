#include "CorrectSystem.h"
#include "../../Player.h"
#include "../../../ObjectList.h"
#include "../../../../../Engine/Math/Matrix3x3.h"
#include "../../../Engine/Input/Input.h"
#include "../../../Engine/GlobalVariables/GlobalVariables.h"

void CorrectSystem::Initialize(Player* player)
{
	// ポインタ登録
	player_ = player;

	// アシストの値設定
	SetAssistValue();

}

void CorrectSystem::Update(EnemyManager* enemyManager)
{
	// エイムアシストの段階
	if (assistLevel_ == AssistLevel::kNone) {
		// システムをオンに
		isSystem_ = false;
		// ターゲット消す
		targetPointer_ = nullptr;
	}
	else if (assistLevel_ == AssistLevel::kNormal) {
		// システムをオンに
		isSystem_ = true;
		// アシストの値変更
		SetAssistValue();
	}
	else if (assistLevel_ == AssistLevel::kHard) {
		// システムをオンに
		isSystem_ = true;
		// アシストの値変更
		SetAssistValue();
	}
	else {
		// システムをオンに
		isSystem_ = true;
	}

	Input* input = Input::GetInstance();
	Vector2 rightStick = input->GetRightAnalogstick();
	Vector2 leftStick = input->GetLeftAnalogstick();

	if (isSystem_ && targetPointer_) {
		isLockOn_ = true;
		// 長さを作成
		float length = Vector3::Length(targetPointer_->GetWorldPosition() - player_->worldtransform_.GetWorldPosition());
		// 長さが短いか確認
		if (length < assistValue_.InitLength_) {
			isInNearArea_ = true;
			targetDirect_ = targetPointer_->GetWorldPosition() - player_->worldtransform_.GetWorldPosition();
		}
		else {
			targetPointer_ = nullptr;
		}
	}
	else if(isSystem_ && !targetPointer_){
		isLockOn_ = false;
		NearLockOn(enemyManager);
	}
	// スティックを倒した時にロックオンを解除する際の傾きのデッドゾーンの値
	float lockCancell = 0.75f;
	if (rightStick.x != 0 || rightStick.y != 0) {
		// 解除処理
		if (std::fabsf(rightStick.x) > lockCancell || std::fabsf(rightStick.y) > lockCancell) {
			targetPointer_ = nullptr;
		}
		Vector3 normalize = { rightStick.x / SHRT_MAX,rightStick.y / SHRT_MAX,0 };
		normalize.y *= -1.0f;
		// 仮のアシスト君
		if (isSystem_) {
			player_->throwDirect_ = StickAimAssist(enemyManager, normalize);
		}
		else {
			player_->throwDirect_ = normalize;
		}
	}
	else if (leftStick.x != 0 || leftStick.y != 0) {
		// 解除処理
		if (std::fabsf(leftStick.x) > lockCancell || std::fabsf(leftStick.y) > lockCancell) {
			targetPointer_ = nullptr;
		}
		Vector3 normalize = { leftStick.x / SHRT_MAX,leftStick.y / SHRT_MAX,0 };
		normalize.y *= -1.0f;
		// 仮のアシスト君
		if (isSystem_) {
			player_->throwDirect_ = LeftStickAimAssist(enemyManager, normalize);
		}
		else {
			player_->throwDirect_ = normalize;
		}
	}
	else {
		player_->throwDirect_ = targetDirect_;
	}
	prevLeftStick_ = leftStick;

	isInNearArea_ = false;
}

void CorrectSystem::ImGuiDraw()
{
	ImGui::Text("isInNearArea : %d", isInNearArea_);
	ImGui::Text("isLock : %d", isLockOn_);
	ImGui::DragFloat3("TVect", &targetDirect_.x);

	ImGui::Checkbox("isLock", &isSystem_);
	int level = assistLevel_;
	ImGui::DragInt("Level", &level, 1);
	assistLevel_ = level;
}

void CorrectSystem::TargetStop()
{
	if (targetPointer_) {
		static_cast<Enemy*>(targetPointer_)->SetIsMoveStop(true);
	}

}

void CorrectSystem::SetAssistValue()
{
	if (assistLevel_ == AssistLevel::kNormal) {
		// 最低の長さ
		assistValue_.InitLength_ = GlobalVariables::GetInstance()->GetFloatValue("AimCorrection", "InitLength_Normal");
		// 幅
		assistValue_.rotationWidth_ = GlobalVariables::GetInstance()->GetFloatValue("AimCorrection", "RotateWidth_Normal");
		// エイムアシストの幅
		assistValue_.assistWidth_ = GlobalVariables::GetInstance()->GetFloatValue("AimCorrection", "AssistWidth_Normal");
	}
	else if(assistLevel_ == AssistLevel::kHard){
		// 最低の長さ
		assistValue_.InitLength_ = GlobalVariables::GetInstance()->GetFloatValue("AimCorrection", "InitLength_Hard");
		// 幅
		assistValue_.rotationWidth_ = GlobalVariables::GetInstance()->GetFloatValue("AimCorrection", "RotateWidth_Hard");
		// エイムアシストの幅
		assistValue_.assistWidth_ = GlobalVariables::GetInstance()->GetFloatValue("AimCorrection", "AssistWidth_Hard");
	}
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
	// 方向ベクトル
	Vector3 direction = { 0.0f, 0.0f,0.0f };
	// 目指す方向ベクトル
	Vector3 targetDirection = player_->throwDirect_;

	// 方向確認用のベクトル(プレイヤー)
	Vector2 playerDirection = { player_->throwDirect_.x, player_->throwDirect_.y };

	Matrix3x3 leftRotateMatrix = Matrix3x3::MakeRotateMatrix(-assistValue_.rotationWidth_);
	// 方向確認用の行列(右範囲)
	Matrix3x3 rightRotateMatrix = Matrix3x3::MakeRotateMatrix(assistValue_.rotationWidth_);
	// 方向確認用のベクトル(エネミー)
	Vector2 enemyDirection = { 0.0f, 0.0f };

	// クロス積左
	float leftCross = 0.0f;
	// クロス積右
	float rightCross = 0.0f;

	float lengthMin = assistValue_.InitLength_;

	// ループ文
	std::list<std::unique_ptr<LargeNumberOfObjects>>::iterator emitItr = enemyManager->GetEmitterLists()->begin();
	for (; emitItr != enemyManager->GetEmitterLists()->end(); ++emitItr) {
		//static_cast<LargeNumberOfObjects>
		std::list<std::unique_ptr<OneOfManyObjects>>::iterator it = (*emitItr)->GetObjects()->begin();
		for (; it != (*emitItr)->GetObjects()->end(); ++it) {
			// エネミーをとってくる
			OneOfManyObjects* obj = it->get();
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
					targetPointer_ = obj;
				}
			}

		}
	}

	targetDirect_ = targetDirection;
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
	// 方向ベクトル
	Vector3 direction = { 0.0f, 0.0f,0.0f };
	// 目指す方向ベクトル
	Vector3 targetDirection = player_->throwDirect_;

	// 方向確認用のベクトル(プレイヤー)
	Vector2 playerDirection = { player_->throwDirect_.x, player_->throwDirect_.y };

	Matrix3x3 leftRotateMatrix = Matrix3x3::MakeRotateMatrix(-assistValue_.rotationWidth_);
	// 方向確認用の行列(右範囲)
	Matrix3x3 rightRotateMatrix = Matrix3x3::MakeRotateMatrix(assistValue_.rotationWidth_);
	// 方向確認用のベクトル(エネミー)
	Vector2 enemyDirection = { 0.0f, 0.0f };

	// クロス積左
	float leftCross = 0.0f;
	// クロス積右
	float rightCross = 0.0f;

	// 現在の最小の長さ
	float lengthMin = assistValue_.InitLength_;

	// ループ文
	std::list<std::unique_ptr<LargeNumberOfObjects>>::iterator emitItr = enemyManager->GetEmitterLists()->begin();
	for (; emitItr != enemyManager->GetEmitterLists()->end(); ++emitItr) {
		std::list<std::unique_ptr<OneOfManyObjects>>::iterator it = (*emitItr)->GetObjects()->begin();
		for (; it != (*emitItr)->GetObjects()->end(); ++it) {
			// エネミーをとってくる
			OneOfManyObjects* obj = it->get();
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
	}

	return targetDirection;
}

Vector3 CorrectSystem::StickAimAssist(EnemyManager* enemyManager, const Vector3& stickDirect)
{
	// プレイヤーのワールドポジション
	Vector3 playerPos = player_->worldtransform_.GetWorldPosition();
	// エネミーのワールドポジション
	Vector3 enemyPos = { 0.0f,0.0f,0.0f };
	// エネミーへのベクトル
	Vector3 toEnemy = { 0.0f,0.0f,0.0f };
	// 長さ
	float length = 0.0f;

	// 方向ベクトル
	Vector3 direction = { 0.0f, 0.0f,0.0f };
	// 目指す方向ベクトル
	Vector3 targetDirection = stickDirect;

	// 方向確認用のベクトル(プレイヤー)
	Vector2 playerDirection = { stickDirect.x, stickDirect.y };

	Matrix3x3 leftRotateMatrix = Matrix3x3::MakeRotateMatrix(-assistValue_.assistWidth_);
	// 方向確認用の行列(右範囲)
	Matrix3x3 rightRotateMatrix = Matrix3x3::MakeRotateMatrix(assistValue_.assistWidth_);
	// 方向確認用のベクトル(エネミー)
	Vector2 enemyDirection = { 0.0f, 0.0f };

	// クロス積左
	float leftCross = 0.0f;
	// クロス積右
	float rightCross = 0.0f;

	// 現在の最小の長さ
	float lengthMin = assistValue_.InitLength_;

	// ループ文
	std::list<std::unique_ptr<LargeNumberOfObjects>>::iterator emitItr = enemyManager->GetEmitterLists()->begin();
	for (; emitItr != enemyManager->GetEmitterLists()->end(); ++emitItr) {
		std::list<std::unique_ptr<OneOfManyObjects>>::iterator it = (*emitItr)->GetObjects()->begin();
		for (; it != (*emitItr)->GetObjects()->end(); ++it) {
			// エネミーをとってくる
			OneOfManyObjects* obj = it->get();
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

				if (leftCross <= 0.0f && rightCross > 0.0f) {
					isInNearArea_ = true;
					// 目指す方向ベクトルを更新
					targetDirection = direction;
					// 現在の最小の長さを更新
					lengthMin = length;
					targetPointer_ = obj;
				}
			}
		}
	}

	return Vector3::Normalize(targetDirection);
}

Vector3 CorrectSystem::LeftStickAimAssist(EnemyManager* enemyManager, const Vector3& stickDirect)
{
	// プレイヤーのワールドポジション
	Vector3 playerPos = player_->worldtransform_.GetWorldPosition();
	// エネミーのワールドポジション
	Vector3 enemyPos = { 0.0f,0.0f,0.0f };
	// エネミーへのベクトル
	Vector3 toEnemy = { 0.0f,0.0f,0.0f };
	// 長さ
	float length = 0.0f;

	// 方向ベクトル
	Vector3 direction = { 0.0f, 0.0f,0.0f };
	// 目指す方向ベクトル
	Vector3 targetDirection = stickDirect;

	// 方向確認用のベクトル(プレイヤー)
	Vector2 playerDirection = { stickDirect.x, stickDirect.y };

	Matrix3x3 leftRotateMatrix = Matrix3x3::MakeRotateMatrix(-assistValue_.assistWidth_);
	// 方向確認用の行列(右範囲)
	Matrix3x3 rightRotateMatrix = Matrix3x3::MakeRotateMatrix(assistValue_.assistWidth_);
	// 方向確認用のベクトル(エネミー)
	Vector2 enemyDirection = { 0.0f, 0.0f };

	// クロス積左
	float leftCross = 0.0f;
	// クロス積右
	float rightCross = 0.0f;

	// 現在の最小の長さ
	float lengthMin = assistValue_.InitLength_;

	// ループ文
	std::list<std::unique_ptr<LargeNumberOfObjects>>::iterator emitItr = enemyManager->GetEmitterLists()->begin();
	for (; emitItr != enemyManager->GetEmitterLists()->end(); ++emitItr) {
		std::list<std::unique_ptr<OneOfManyObjects>>::iterator it = (*emitItr)->GetObjects()->begin();
		for (; it != (*emitItr)->GetObjects()->end(); ++it) {
			// エネミーをとってくる
			OneOfManyObjects* obj = it->get();
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

				if (leftCross <= 0.0f && rightCross > 0.0f && ((toEnemy.x > 0 && player_->worldtransform_.direction_.x > 0) || (toEnemy.x < 0 && player_->worldtransform_.direction_.x < 0))) {
					isInNearArea_ = true;
					// 目指す方向ベクトルを更新
					targetDirection = direction;
					// 現在の最小の長さを更新
					lengthMin = length;
					targetPointer_ = obj;
				}
			}
		}
	}
	return Vector3::Normalize(targetDirection);
}
