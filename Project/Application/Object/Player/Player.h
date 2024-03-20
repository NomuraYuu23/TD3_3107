#pragma once
#include "../IObject.h"
#include "../Weapon/Weapon.h"
#include "../GameUtility/TimerLib.h"
#include "../../../Engine/Input/Input.h"

#include "ActionState/ActionStateList.h"
#include "System/PlayerController.h"
#include "System/Recoil/PlayerRecoil.h"
#include "PlayerFootCollider.h"

class Player : public IObject
{
private:
	struct FootCollisionData {
		Vector2 position_ = {};
		Vector2 scale_ = {};
		Box collider_ = {};
	};

	struct ArrowUIData {
		Model* plane_;
		Vector3 v3Position;
		float offsetLength;
	};

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

	Vector2 GetColliderPosition() override { return circleCollider_.position_; }
	Vector2 GetColliderSize() override { return boxCollider_.scale_; }
	Box GetBoxCollider() override { return boxCollider_; }
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
	/// 足元のコライダー
	/// </summary>
	/// <returns></returns>
	PlayerFootCollider* GetFootCollider() { return &footCollider_; }

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

	void SetArrowModel(Model* arrow) { arrow_.plane_ = arrow; }

public:
	// ステート
	std::unique_ptr<IActionState> actionState_;
	// 武器
	std::unique_ptr<Weapon> weapon_;
	// 重力
	float gravity_ = 9.8f;
	// 投げる方向
	Vector3 throwDirect_ = { 1,0,0 };
	// 前座標
	Vector3 prevPosition_ = {};

	uint32_t arrowTexture_ = 0u;

	PlayerFootCollider footCollider_;

	bool isGround_ = false;

	float jumpPower_ = 0;

	float spearJumpPower_ = 0;

private:
	// 現状のステート
	PlayerState nowState_;
	// 反動管理クラス
	PlayerRecoil recoil_;
	// 操作クラス
	PlayerController controller_;

	ArrowUIData arrow_;

	bool isSlowGame_ = false;

	float threshold_y_ = 0.17f;

	bool isDebugDraw_ = false;

};

