#include "RunSmoke.h"
#include "../../../../Engine/Math/RandomEngine.h"

void RunSmoke::Initialize(ParticleDesc* particleDesc)
{
	std::random_device seedGenerator;
	std::mt19937 randomEngine(seedGenerator());

	Vector3 distributionMax = { particleDesc->position.x + particleDesc->size.x / 2.0f,
								particleDesc->position.y + particleDesc->size.y / 2.0f,
								particleDesc->position.z + particleDesc->size.z / 2.0f };
	Vector3 distributionMin = { particleDesc->position.x - particleDesc->size.x / 2.0f,
								particleDesc->position.y - particleDesc->size.y / 2.0f,
								particleDesc->position.z - particleDesc->size.z / 2.0f };

	// 最小スケール
	Vector3 minScale = { particleDesc->size.x / 1.5f,
						 particleDesc->size.y / 1.5f,
						 particleDesc->size.z / 1.5f };
	// 最大スケール
	Vector3 maxScale = { particleDesc->size.x * 1.5f,
						 particleDesc->size.y * 1.5f,
						 particleDesc->size.z * 1.5f };

	// 最小ベクトル
	Vector3 minVector = { -0.05f,
						  -0.05f,
						  0.0f };
	// 最大ベクトル
	Vector3 maxVector = { 0.05f,
						  0.05f,
						  0.0f };

	// 最小、最大スケールランダム値
	std::uniform_real_distribution<float> distScale(minScale.x, maxScale.x);
	std::uniform_real_distribution<float> distVectorX(minVector.x, maxVector.x);
	std::uniform_real_distribution<float> distVectorY(minVector.y, maxVector.y);
	std::uniform_real_distribution<float> distColor(0.5f, 1.0f);
	std::uniform_real_distribution<float> distTime(0.15f, 0.35f);

	float distantScale = distScale(randomEngine);
	transform_.scale = { distantScale,distantScale,1.0f };
	transform_.rotate = { 0.0f,0.0f,0.0f };
	transform_.translate = particleDesc->position;

	velocity_ = { distVectorX(randomEngine), distVectorY(randomEngine), 0.0f };

	worldMatrix_ = Matrix4x4::MakeIdentity4x4();

	float c = distColor(randomEngine);
	color_ = { c,c,c, .5f };

	lifeTime_ = distTime(randomEngine);

	currentTime_ = 0.0f;

	useBillBoard_ = true;

	billBoardName_ = kBillBoardNameIndexYAxis;

	UpdateMatrix(Matrix4x4::MakeIdentity4x4());

	isDead_ = false;
}

void RunSmoke::Update(const Matrix4x4& billBoardMatrix)
{
	// 速度ベクトルにしたがって粒子を移動させる
	transform_.translate += velocity_;

	if (transform_.scale.x > 0.0f) {
		transform_.scale.x -= 0.01f;
		transform_.scale.y -= 0.01f;
	}
	else {
		transform_.scale.x = 0.0f;
		transform_.scale.y = 0.0f;
	}

	// 透明度が0以上の時
	if (color_.w >= 0.0f) {
		color_.w -= 0.01f;
	}
	else {
		color_.w = 0.0f;
	}

	// 基底クラス更新
	IParticle::Update(billBoardMatrix);
}
