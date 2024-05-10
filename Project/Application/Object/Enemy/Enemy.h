#pragma once
#include "../IObject.h"
#include "State/EnemyStateList.h"
#include "../../../Engine/3D/OneOfManyObjects.h"
#include "IEnemyEmitter.h"

class Enemy : public OneOfManyObjects
{
private:
	// 共通
	static uint32_t sSerialNumber_;
	// シリアルナンバー
	uint32_t serialNum_ = 0;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// ImGui
	/// </summary>
	void ImGuiDraw();

	/// <summary>
	/// 衝突のコールバック
	/// </summary>
	/// <param name="target"></param>
	/// <param name="tag"></param>
	void OnCollision(ColliderParentObject2D target);

	Vector2 GetColliderPosition() { return boxCollider_.position_; }
	Vector2 GetColliderSize() { return boxCollider_.scale_; }
	Box GetBoxCollider() { return boxCollider_; }

	void BoxColliderUpdate() {
		boxCollider_.Update(position2D_, scale2D_.x, scale2D_.y, 0.0f);
	}

	bool IsGround() { return isGround_; }

	void MatrixUpdate();

	void SetEmitter(IEnemyEmitter* parent) {
		parentEmitter_ = parent;
		parent_ = parentEmitter_->GetWorldTransform();
	}

public:
	/// <summary>
	/// 生成時に呼び出す関数（ここで地上・空中の選択、その際に近接・遠隔の選択も
	/// </summary>
	void GenerateSetting();

	/// <summary>
	/// ステートの初期化
	/// </summary>
	/// <param name="newState"></param>
	/// <param name="attackPattern"></param>
	void StateInitialize(std::unique_ptr<IEnemyState> newState, uint32_t attackPattern);

	//void ChangeRequest()

	void SetState(IEnemyState* state) {
		judState_ = state;
	}

private:

	void ChangeState(std::unique_ptr<IEnemyState> newState, IEnemyState::AttackPattern pattern);

	

private:
	std::string name_;

	EnemyState judState_;

	bool isGround_ = false;
private:
	// 状態
	std::unique_ptr<IEnemyState> state_;

public:

	// コライダー用の座標・スケール
	Vector2 position2D_ = {};
	Vector2 scale2D_ = {};

	Vector2 prevPosition_ = {};

	// コライダー
	Box boxCollider_;

	// 速度
	Vector3 velocity_ = {};

	//// 親の座標
	//Vector3 parentPosition_;
	IEnemyEmitter* parentEmitter_;
};
