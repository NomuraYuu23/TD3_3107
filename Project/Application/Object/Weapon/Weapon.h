#pragma once
#include "../IObject.h"
#include "../GameUtility/TimerLib.h"
#include "WeaponState/WeaponStateList.h"
#include "WeaponState/StateList.h"

#include "../Map/Terrain.h"

#include "../../../Engine/GlobalVariables/GlobalVariables.h"

class Player;

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
		kFreeFall,	// 自由落下
		//kCount,
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
	void Draw(const BaseCamera& camera) override;
	/// <summary>
	/// ImGui
	/// </summary>
	void ImGuiDraw() override;
	/// <summary>
	/// 衝突コールバック
	/// </summary>
	/// <param name="target"></param>
	/// <param name="tag"></param>
	void OnCollision(ColliderParentObject2D target) override;

	void SetPlayer(Player* player) { player_ = player; }

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
	bool IsTread() { return isTread_; }

	bool IsGravity() { return isGravity_; }

	void SetIsGravity(bool isGravity) { isGravity_ = isGravity; }

	float GetReturnRate() { return returnRate_; }
	Vector2 GetColliderPosition() override { return boxCollider_.position_; }
	Vector2 GetColliderSize() override { return boxCollider_.scale_; }
	Box GetBoxCollider() override { return boxCollider_; }
	WeaponState GetNowState() { return nowState_; }

	void SetState(WeaponState newState) { nowState_ = newState; }

	bool IsEnemyImpaled() { return isEnemyImpaled_; }

	bool IsPlayerJump() { return isPlayerJumpAccept_; }

public: // 外部で行う設定関数
	/// <summary>
	/// 変更のリクエスト
	/// </summary>
	/// <param name="request"></param>
	void ChangeRequest(Weapon::StateName request);

	/// <summary>
	/// 親の設定
	/// </summary>
	/// <param name="adress"></param>
	void SettingParent() {
		worldtransform_.SetParent(parentAdress_);
		worldtransform_.transform_.translate = GlobalVariables::GetInstance()->GetVector3Value("Weapon", "LocalPosition");
	}

	/// <summary>
	/// 親子の設定解除
	/// </summary>
	void ReleaseParent() {
		worldtransform_.transform_.translate = parentAdress_->GetWorldPosition();
		worldtransform_.SetParent(nullptr);
		//worldtransform_.transform_.translate = worldtransform_.GetWorldPosition();
		worldtransform_.UpdateMatrix();
	}

	/// <summary>
	/// 踏まれた際の初期設定
	/// </summary>
	void TreadSetting();

	// 投げる方向
	Vector3 throwDirect_ = {};
	// 帰ってくる方向
	Vector2 returnDirect_ = {};
	// 刺さった時の逆ベクトル
	Vector2 invDirect_ = {};

	Vector2 prevDirect_ = {};

	// 引き寄せの最初の衝突しない時間
	TimerLib attractInvTimer_;

	// 投げの最初の衝突無効処理
	TimerLib throwInvTimer_;

	// 刺さってる武器が自動的に戻る大麻ー
	TimerLib autoComeBackTimer_;

	Terrain::BlockType hitBlockType_ = Terrain::BlockType::kNone;

private:
	/// <summary>
	/// ステート変更
	/// </summary>
	/// <param name="newState"></param>
	void ChangeState(std::unique_ptr<IWeaponState> newState);

private:
	// ステート
	std::unique_ptr<IWeaponState> state_;
	// 親のワールドトランスフォーム
	WorldTransform* parentAdress_ = nullptr;
	// 一回踏んだか確認フラグ
	bool isTread_ = false;
	// 重力フラグ
	bool isGravity_ = false;
	// 帰ってくるときの座標レート
	float returnRate_ = 1.0f;

	// タイマーライブラリ
	TimerLib timer_;
	// コライダーチェック用のフラグ
	bool isCollisionCheck_ = false;
	// 角度
	float rotateAngle_ = 0;

	// 現在のステート
	WeaponState nowState_;

	float dotAngle_ = 0;

	bool isEnemyImpaled_ = false;

	Player* player_ = nullptr;
	// 槍ジャンプを行ったかどうか
	bool isPlayerJumpAccept_ = false;

};

