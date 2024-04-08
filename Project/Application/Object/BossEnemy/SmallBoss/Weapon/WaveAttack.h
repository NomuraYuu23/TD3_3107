#pragma once
#include "../../../IObject.h"
#include "../../Common/Weapon/IAttackObject.h"


class WaveAttack : public IAttackObject
{
public:
	WaveAttack() {};

public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	void Initialize(Model* model) override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	void Draw(const BaseCamera& camera) override;
	/// <summary>
	/// ImGui
	/// </summary>
	void ImGuiDraw() override;
	/// <summary>
	/// 衝突のコールバック
	/// </summary>
	/// <param name="target"></param>
	/// <param name="tag"></param>
	void OnCollision(ColliderParentObject2D target) override;

public: // アクセッサ
	//Box GetBoxCollider() override{
	//	return boxCollider_;
	//}
	Vector2 GetColliderPosition() override {
		return boxCollider_.position_;
	}
	Vector2 GetColliderSize() override {
		return boxCollider_.scale_;
	}

	void CreateVelocity(const Vector3& direct) {
		velocity_ = direct;
		velocity_ *= movePower_;
	}

private:
	Vector3 velocity_ = {};
	float movePower_ = 0;
};