#pragma once
#include "../../../Engine/Math/Vector3.h"
#include "../../../Engine/Math/DeltaTime.h"

class IBoss;

class IBossState
{
public: // コンストラクタ
	virtual ~IBossState() = default;

public:
	/// <summary>
	/// ポインタの設定などの初期化の前
	/// </summary>
	/// <param name="player"></param>
	void PreInitialize(IBoss* boss);

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize() = 0;

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update();

protected:
	// ボスのポインタ
	IBoss* boss_ = nullptr;


};