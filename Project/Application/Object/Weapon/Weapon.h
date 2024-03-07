#pragma once
#include "../IObject.h"
#include "WeaponState/WeaponStateList.h"

class Weapon : public IObject
{
public: // サブクラス
	// ステートごとの名前（変更時に使う
	enum class StateName : int {

		kHold,		// 持っている
		kThrown,	// 投げられている
		kImpaled,	// 刺さっている
		kReturn,	// 帰ってきてる

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
	void OnCollision(ColliderParentObject2D* target, uint32_t tag) override;

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
	void ChangeRequest(Weapon::StateName request) {
		switch (request)
		{
		case Weapon::StateName::kHold:
			ChangeState(std::make_unique<HoldState>());
			break;
		case Weapon::StateName::kThrown:
			ChangeState(std::make_unique<ThrownState>());
			break;
		case Weapon::StateName::kImpaled:
			ChangeState(std::make_unique<ImpaledState>());
			break;
		case Weapon::StateName::kReturn:
			ChangeState(std::make_unique<ReturnState>());
			break;
		}
	}

	/// <summary>
	/// 親のトランスフォームアドレス格納
	/// </summary>
	/// <param name="adress"></param>
	void SetParentAdress(WorldTransform* adress) { parentAdress_ = adress; }

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
	/// 親のワールド座標
	/// </summary>
	/// <returns></returns>
	Vector3 GetTargetPosition() { return parentAdress_->GetWorldPosition(); }
	
	// 投げる方向
	Vector3 throwDirect_ = {};

private:
	// 現在の状態
	std::unique_ptr<IWeaponState> state_;
	// 親のワールドトランスフォーム
	WorldTransform* parentAdress_ = nullptr;
	// 動かすオフセット座標
	Vector3 localOffset_ = {};

};

