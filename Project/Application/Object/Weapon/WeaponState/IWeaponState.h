#pragma once
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

	/// <summary>
	/// カウント開始
	/// </summary>
	/// <param name="frame"></param>
	virtual void StartEasing(int frame);
protected:
	/// <summary>
	/// イージングの更新
	/// </summary>
	virtual void EaseUpdate();

protected:
	// 武器
	Weapon* weapon_ = nullptr;
	// 線形補間用のT
	float ease_t_ = 0;
	// 終了フレーム数
	float easingEndFrame_ = 0;
	// 終了タイミング取得
	bool isEnd_ = false;
protected:
	//// イージング中か
	//bool isEasing_ = false;

};

