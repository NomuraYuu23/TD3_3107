#pragma once
#include "../IObject.h"
#include "State/EnemyStateList.h"
#include "../../../Engine/3D/OneOfManyObjects.h"
#include "IEnemyEmitter.h"
#include "../GameUtility/TimerLib.h"

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

	/// <summary>
	/// エミッター設定
	/// </summary>
	/// <param name="parent"></param>
	inline void SetEmitter(IEnemyEmitter* parent) {
		parentEmitter_ = parent;
		parent_ = parentEmitter_->GetWorldTransform();
	}

	/// <summary>
	/// 親子設定しなおし
	/// </summary>
	inline void ResetParent() {
		SetParent(parentEmitter_->GetWorldTransform());
		transform_.translate = defaultOffset_;
		MatrixUpdate();
	}

	/// <summary>
	/// 親子解除
	/// </summary>
	inline void ReleaseParent() {
		transform_.translate = GetWorldPosition();
		SetParent(nullptr);
		MatrixUpdate();
		if (parentEmitter_) {
			goalAngle_ = parentEmitter_->GetNowAngle();
		}

		interval_.Start(5.0f);
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

	void SetState(EnemyState state) {
		judState_ = state;
	}

	void SetDefaultOffset(const Vector3& offset) { defaultOffset_ = offset; }
	IEnemyState* GetNowState() { return state_.get(); }
	EnemyState GetState() { return judState_; }

private:

	void ChangeState(std::unique_ptr<IEnemyState> newState, IEnemyState::AttackPattern pattern);

	void CheckParent();

private:
	// 名前
	std::string name_;

	// 現状の状態
	EnemyState judState_;

	// 接地フラグ
	bool isGround_ = false;

	// 武器の解除用フラグ
	bool isRealeseActive_ = false;

private:
	// 状態
	std::unique_ptr<IEnemyState> state_;

	Weapon* weapon_ = nullptr;

public:

	// コライダー用の座標・スケール
	Vector2 position2D_ = {};
	Vector2 scale2D_ = {};

	Vector2 prevPosition_ = {};

	// 敵の移動方向を求めるための前フレーム座標
	Vector3 prevTranslate_ = {};

	// コライダー
	Box boxCollider_;

	// 速度
	Vector3 velocity_ = {};

	//// 親の座標
	IEnemyEmitter* parentEmitter_;

	Vector3 defaultOffset_;

	float goalAngle_ = 0.0f;

	TimerLib interval_;

};
