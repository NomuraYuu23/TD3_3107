#pragma once
#include "../../../GameUtility/TimerLib.h"
#include "../../../Engine/Math/Vector3.h"

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
	void Cancel() { compTimer_.End(); }

	void ImGuiDraw();

private:
	void SetUp();
	float scaling_ = 0.5f;
	float maxSize_ = 0.0f;
private: // 必要なオブジェクトのポインタ
	Player* player_ = nullptr;
	Weapon* weapon_ = nullptr;

	// 
	bool isInRange_ = false;

	TimerLib compTimer_;

	Vector3 startPosition_ = {};

};
// ジャンプの落下中かつ武器とプレイヤーのが一定の距離ならそちらに引き寄せて衝突させる