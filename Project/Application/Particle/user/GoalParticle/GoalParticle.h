#pragma once
#include "../../../Engine//Particle/IParticle.h"

/// <summary>
/// ゴールから放出されるパーティクル
/// </summary>
class GoalParticle : public IParticle
{
public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="position">初期座標</param>
	/// <param name="size">大きさ</param>
	void Initialize(ParticleDesc* particleDesc) override;

	/// <summary>
	/// 更新関数
	/// </summary>
	/// <param name="cameraMatrix4x4">カメラ行列</param>
	void Update(const Matrix4x4& billBoardMatrix) override;

private: // メンバ変数
};

