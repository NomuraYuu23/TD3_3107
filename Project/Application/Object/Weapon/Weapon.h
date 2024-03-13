#pragma once
#include "../IObject.h"
#include "WeaponState/WeaponStateList.h"
#include "WeaponState/StateList.h"
#include "../GameUtility/TimerLib.h"

class Weapon : public IObject
{
public: // サブクラス
	// ステートごとの名前（変更時に使う
	enum class StateName : int {

		kHold,		// 持っている
		kThrown,	// 投げられている
		kImpaled,	// 刺さっている
		kReturn,	// 帰ってきてる
		kWait,		// 壁での待機時間
		//kCount,
	};

	// 現在のステート
	WeaponState nowState_;

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
	/// 衝突コールバック
	/// </summary>
	/// <param name="target"></param>
	/// <param name="tag"></param>
	void OnCollision(ColliderParentObject2D target, const Vector2& targetPosition) override;
	Vector2 GetCollierPosition() override { return boxCollider_.position_; }

private:
	/// <summary>
	/// ステート変更
	/// </summary>
	/// <param name="newState"></param>
	void ChangeState(std::unique_ptr<IWeaponState> newState);

public: // メンバ関数
	/// <summary>
	/// 変更のリクエスト
	/// </summary>
	/// <param name="request"></param>
	void ChangeRequest(Weapon::StateName request);

public: // アクセッサ
	/// <summary>
	/// 親のトランスフォームアドレス格納
	/// </summary>
	/// <param name="adress"></param>
	void SetParentAdress(WorldTransform* adress) { parentAdress_ = adress; }

	/// <summary>
	/// 親のワールド座標
	/// </summary>
	/// <returns></returns>
	Vector3 GetTargetPosition() { return parentAdress_->GetWorldPosition(); }
	/// <summary>
	/// 踏まれたフラグ取得
	/// </summary>
	/// <returns></returns>
	bool GetIsTread() { return isTread_; }

	bool GetIsGravity() { return isGravity_; }

	float GetReturnRate() { return returnRate_; }

public: // 外部で行う設定関数
	/// <summary>
	/// 親の設定
	/// </summary>
	/// <param name="adress"></param>
	void SettingParent() {
		worldtransform_.SetParent(parentAdress_);
		worldtransform_.transform_.translate = localOffset_;
	}

	/// <summary>
	/// 親子の設定解除
	/// </summary>
	void ReleaseParent() {
		worldtransform_.SetParent(nullptr);
		worldtransform_.transform_.translate = worldtransform_.GetWorldPosition();
		worldtransform_.UpdateMatrix();
	}

	/// <summary>
	/// 踏まれた際の初期設定
	/// </summary>
	void TreadSetting();

	/// <summary>
	/// 重力が掛かる際の初期化
	/// </summary>
	void GravityInitialize() {
		isGravity_ = true;
	}


	// 投げる方向
	Vector3 throwDirect_ = {};
	// 帰ってくる方向
	Vector2 returnDirect_ = {};


private:
	// ステート
	std::unique_ptr<IWeaponState> state_;
	// 親のワールドトランスフォーム
	WorldTransform* parentAdress_ = nullptr;
	// 動かすオフセット座標
	Vector3 localOffset_ = {};
	// 一回踏んだか確認フラグ
	bool isTread_ = false;
	// 帰ってくるときの座標レート
	float returnRate_ = 1.0f;

	// タイマーライブラリ
	TimerLib timer_;

	// 重力フラグ
	bool isGravity_ = false;
	// 重力の値
	float gravityValue_ = 0;

};

