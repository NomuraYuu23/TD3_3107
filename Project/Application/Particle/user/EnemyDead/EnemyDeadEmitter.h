#pragma once
#include "../../../../Engine/Particle/IEmitter.h"

/// <summary>
/// 敵の死亡パーティクルエミッター
/// </summary>
class EnemyDeadEmitter : public IEmitter
{
public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="emitterDesc">エミッタ設定構造体</param>
	void Initialize(const EmitterDesc& emitterDesc) override;

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update() override;

	/// <summary>
	/// パーティクル生成関数
	/// </summary>
	/// <returns>生成済みパーティクルリスト</returns>
	std::list<IParticle*> Emit() override;
};

