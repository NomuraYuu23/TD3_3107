#pragma once
#include "../../../Engine//Particle/IParticle.h"
#include "../../../Engine/Math/Vector2.h"

/// <summary>
/// 死亡時に発生する集束パーティクル
/// </summary>
class DeadForcusingParticle : public IParticle
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

	// 線形補間用
	Vector3 prePos_;
	Vector3 postPos_;

};

