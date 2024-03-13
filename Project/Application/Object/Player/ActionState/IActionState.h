#pragma once
#include "../../../Engine/Math/Vector3.h"
#include "../../../Engine/Math/DeltaTime.h"

class Player;

class IActionState
{
public: // コンストラクタ
	virtual ~IActionState() = default;

public:
	/// <summary>
	/// ポインタの設定などの初期化の前
	/// </summary>
	/// <param name="player"></param>
	void PreInitialize(Player* player);

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize() = 0;

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update() = 0;

public:
	/// <summary>
	/// 落下中フラグ
	/// </summary>
	/// <param name="isFall"></param>
	void SetIsFall(bool isFall) { isFall_ = isFall; }

protected:
	// プレイヤーのポインタ
	Player* player_ = nullptr;

	// 落下中か
	bool isFall_ = false;

};

