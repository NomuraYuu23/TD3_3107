#pragma once
#include "StateList.h"
#include "../../../Engine/2D/ImguiManager.h"

class Weapon;

class IWeaponState
{
public:
	virtual ~IWeaponState() = default;

public:
	/// <summary>
	/// 前初期化（ポインタの設定）
	/// </summary>
	/// <param name="weapon"></param>
	void PreInitialize(Weapon* weapon);
	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize() = 0;
	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;
	/// <summary>
	/// ImGuiの更新
	/// </summary>
	virtual void ImGuiUpdate() = 0;

public: //アクセッサ
	/// <summary>
	/// 現状のステート設定
	/// </summary>
	/// <param name="state"></param>
	void SetNowState(WeaponState state);

protected:
	// 武器
	Weapon* weapon_ = nullptr;
	// 終了フラグ
	bool isEnd_ = false;
};

