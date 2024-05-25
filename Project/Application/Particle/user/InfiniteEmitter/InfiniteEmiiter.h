#pragma once
#include "../../../Engine/Particle/IEmitter.h"

/// <summary>
/// シーン切り替えまで無限生成するエミッタ
/// </summary>
class InfiniteEmiiter : public IEmitter
{
public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(EmitterDesc* emitterDesc) override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 粒子生成
	/// </summary>
	/// <returns>パーティクルのリスト</returns>
	std::list<IParticle*> Emit() override;

private: // メンバ変数

	// 生成で使うトランスフォームポインタ
	EulerTransform* emitTransform_ = nullptr;

};

