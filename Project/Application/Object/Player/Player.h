#pragma once
#include "../IObject.h"
#include "../Weapon/Weapon.h"
#include "../GameUtility/TimerLib.h"
#include "../../../Engine/Input/Input.h"

#include "ActionState/ActionStateList.h"
#include "System/PlayerController.h"
#include "System/Recoil/PlayerRecoil.h"

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
	void OnCollision(ColliderParentObject2D target, const Vector2& targetPosition) override;

	Vector2 GetCollierPosition() override { return circleCollider_.position_; }

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

	/// <summary>
	/// 武器のポインタ取得
	/// </summary>
	Weapon* GetWeapon() { return weapon_.get(); }

	/// <summary>
	/// 現在の状態
	/// </summary>
	/// <returns></returns>
	PlayerState GetNowState() { return nowState_; }

	/// <summary>
	/// ステートの設定
	/// </summary>
	/// <param name="state"></param>
	void SetNowState(PlayerState state) { nowState_ = state; }

	/// <summary>
	/// 反動中か
	/// </summary>
	bool IsRecoil() { return recoil_.IsActive(); }

public:
	// ステート
	std::unique_ptr<IActionState> actionState_;
	// 武器
	std::unique_ptr<Weapon> weapon_;
	// 速さベクトル
	Vector3 velocity_ = {};
	// 重力
	float gravity_ = 9.8f;
	// 投げる方向
	Vector3 throwDirect_ = { 1,0,0 };

	Vector3 prevPosition_ = {};

private:
	// 現状のステート
	PlayerState nowState_;
	// 反動管理クラス
	PlayerRecoil recoil_;
	// 操作クラス
	PlayerController controller_;

};

