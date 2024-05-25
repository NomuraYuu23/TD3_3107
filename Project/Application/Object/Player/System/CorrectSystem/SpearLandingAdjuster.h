#pragma once
#include "../../../GameUtility/TimerLib.h"
#include "../../../Engine/Math/Vector3.h"
#include "../../../Engine/Math/Vector2.h"

class Player;
class Weapon;

//
class SpearLandingAdjuster
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="player"></param>
	/// <param name="weapon"></param>
	void Initialize(Player* player, Weapon* weapon);

	void Update();
	/// <summary>
	/// 中断
	/// </summary>
	void Cancel() {
		compTimer_.End();
		isRunOnce_ = false;
	}

	bool IsActive() { return compTimer_.IsActive(); }

	void ImGuiDraw();

private:
	void SetUp();
	float scaling_ = 0.5f;
	Vector2 maxSize_ = {};
private: // 必要なオブジェクトのポインタ
	Player* player_ = nullptr;
	Weapon* weapon_ = nullptr;

	// 
	bool isInRange_ = false;
	// 一度起動したか？
	bool isRunOnce_ = false;

	TimerLib compTimer_;

	Vector3 startPosition_ = {};

};
// ジャンプの落下中かつ武器とプレイヤーのが一定の距離ならそちらに引き寄せて衝突させる