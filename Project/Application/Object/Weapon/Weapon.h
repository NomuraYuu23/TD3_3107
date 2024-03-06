#pragma once
#include "../IObject.h"
#include "WeaponState/WeaponStateList.h"

class Weapon : public IObject
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
	/// 衝突コールバック
	/// </summary>
	/// <param name="target"></param>
	/// <param name="tag"></param>
	void OnCollision(ColliderParentObject2D* target, uint32_t tag) override;


public: // メンバ関数
	/// <summary>
	/// ステート変更
	/// </summary>
	/// <param name="newState"></param>
	void ChangeState(std::unique_ptr<IWeaponState> newState);

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
	void ReleaseParent() {
		worldtransform_.SetParent(nullptr);
		worldtransform_.transform_.translate = worldtransform_.GetWorldPosition();
		worldtransform_.UpdateMatrix();
	}

	Vector3 GetTargetPosition() { return parentAdress_->GetWorldPosition(); }

private:
	// 現在の状態
	std::unique_ptr<IWeaponState> state_;

	WorldTransform* parentAdress_ = nullptr;

	Vector3 localOffset_ = {};
};

