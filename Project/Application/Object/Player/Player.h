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
	}

public:
	// 入力
	Input* input_ = nullptr;

	// ステート
	std::unique_ptr<IActionState> actionState_;
	std::unique_ptr<Weapon> weapon_;

	bool stateCheck_ = false;
};

