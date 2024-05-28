#pragma once
#include "IEnemyState.h"
#include "../../../Engine/Math/Vector3.h"
#include "../../GameUtility/TimerLib.h"
#include "../../../Engine/Math/Ease.h"
#include "../System/EnemySytemList.h"

class ChaseEnemyState	 : public IEnemyState
{
private:
	struct PointInfo {
		Vector3 position;
		bool isStart;
	};

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// プレイヤーに追従するシステム
	/// </summary>
	void PlayerChaseUpdate();

	/// <summary>
	/// 追尾用の設定
	/// </summary>
	/// <param name="length"></param>
	void SetChaseSetting(float length);
public:
	// 追尾用のレイ
	DetectionRay sensorRay_;

private:
	// 追尾の判定処理のシステム
	DetectionSystem chaseSystem_;

	float searchLength_ = 0.0f;

};