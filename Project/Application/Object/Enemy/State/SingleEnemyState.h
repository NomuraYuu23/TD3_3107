#pragma once
#include "IEnemyState.h"
#include "../../../Engine/Math/Vector3.h"
#include "../../GameUtility/TimerLib.h"
#include "../../../Engine/Math/Ease.h"

class SingleEnemyState : public IEnemyState
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


	void SettingMoveInfo(const Vector3& endPoint, Ease::EaseName name = Ease::EaseName::Lerp);

private:
	// 近距離
	void PatrolInitialize() override;
	void PatrolUpdate() override;

	// 遠距離
	void ChaseInitialize() override;
	void ChaseUpdate() override;

private:
	// 移動の切り替え
	bool isMoveSwitch_ = false;

	// 移動のチェンジタイマー
	TimerLib moveChangeTimer_;
	// 始点
	PointInfo startPoint_;
	// 終点
	PointInfo endPoint_;
	// 切り返しのフレーム数
	float moveChangeFrame_ = 0;

	// イージングのパターン
	Ease::EaseName easeName_ = Ease::EaseName::Lerp;

private:
	// 行動の関数ポインタを持たせた配列
	inline static const std::array<MovementFunc, static_cast<uint32_t>(ActionMode::kMaxSize)> actionFuncs_{
		MovementFunc{&IEnemyState::PatrolInitialize,&IEnemyState::PatrolUpdate,},
		MovementFunc{&IEnemyState::ChaseInitialize,&IEnemyState::ChaseUpdate,}
	};
};