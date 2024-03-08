#pragma once
#include "../IObject.h"
#include "../../../Engine/Input/Input.h"
#include "ActionState/ActionStateList.h"
#include "../Weapon/Weapon.h"

class Player : public IObject
{
public: // 継承
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
	void OnCollision(ColliderParentObject2D target) override;

public: // メンバ関数
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

	Weapon* GetWeapon() { return weapon_.get(); }

public:
	// 入力
	Input* input_ = nullptr;

	// ステート
	std::unique_ptr<IActionState> actionState_;
	// 武器
	std::unique_ptr<Weapon> weapon_;

	// 速さベクトル
	Vector3 velocity_ = {};
	// 重力
	float gravity_ = 9.8f;
	// 投げる方向
	Vector3 throwDirect_ = {1,0,0};
};

