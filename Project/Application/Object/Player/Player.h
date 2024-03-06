#pragma once
#include "../IObject.h"
#include "../../../Engine/Input/Input.h"
#include "ActionState/ActionStateList.h"
#include "../Weapon/Weapon.h"

class Player : public IObject
{
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
	void Draw(BaseCamera camera) override;
	/// <summary>
	/// ImGui
	/// </summary>
	void ImGuiDraw() override;

	/// <summary>
	/// 衝突判定
	/// </summary>
	/// <param name="target"></param>
	/// <param name="tag"></param>
	void OnCollision(ColliderParentObject2D* target, uint32_t tag) override;

public:
	/// <summary>
	/// ステートの変更
	/// </summary>
	/// <param name="newState"></param>
	void ChangeState(std::unique_ptr<IActionState> newState);

	/// <summary>
	/// 武器の作成（外部で行う想定）
	/// </summary>
	/// <param name="newWeapon"></param>
	void SetWeapon(std::unique_ptr<Weapon> newWeapon) {
		weapon_ = std::move(newWeapon);
		weapon_->SetParentAdress(&worldtransform_);
	}

public:
	// 入力
	Input* input_ = nullptr;

	// ステート
	std::unique_ptr<IActionState> actionState_;
	std::unique_ptr<Weapon> weapon_;

	// 速さベクトル
	Vector3 velocity_ = {};

	float gravity_ = 9.8f;

	Vector3 throwDirect_ = {};
};

