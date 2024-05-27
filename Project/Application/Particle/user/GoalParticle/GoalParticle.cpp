#include "GoalParticle.h"
#include "../../../../Engine/Math/RandomEngine.h"

void GoalParticle::Initialize(ParticleDesc* particleDesc)
{
	// 初期値を取得
	transform_.translate = particleDesc->position; // 座標
	transform_.scale	 = particleDesc->size;	   // 粒子サイズ
	velocity_			 = particleDesc->velocity; // 速度

	// ランダムエンジン生成
	std::random_device seedGenerator;
	std::mt19937 randomEngine(seedGenerator());

	// ランダムな遷移後座標を求める
	// 位置のランダム変数を生成
	Vector3 positionMax = { particleDesc->position.x + 4.5f,
							particleDesc->position.y + 7.5f,
							particleDesc->position.z };
	Vector3 positionMin = { particleDesc->position.x - 4.5f,
							particleDesc->position.y + 5.0f,
							particleDesc->position.z };
	std::uniform_real_distribution<float> randomPostionX(positionMin.x, positionMax.x);
	std::uniform_real_distribution<float> randomPostionY(positionMin.y, positionMax.y);

	Vector3 pos =
	{
		randomPostionX(randomEngine),
		randomPostionY(randomEngine),
		transform_.translate.z,
	};

	// 生存時間のランダム変数を取得
	std::uniform_real_distribution<float> distTime(1.0f, 1.5f);

	// 最初の値からの差分ベクトルを求める
	velocity_ = pos - transform_.translate;
	velocity_ = Vector3::Normalize(velocity_);
	std::uniform_real_distribution<float> distVelocity(0.025f, 0.05f);
	velocity_ = velocity_ * distVelocity(randomEngine);

	// ワールド行列生成
	worldMatrix_ = Matrix4x4::MakeIdentity4x4();

	// 色設定
	std::uniform_real_distribution<float> distColor(0.0f, 1.0f);
	color_ = { distColor(randomEngine),distColor(randomEngine),distColor(randomEngine), 1.0f };

	// 生存時間設定
	lifeTime_ = distTime(randomEngine);

	// 現在時間リセット
	currentTime_ = 0.0f;

	// ビルボードを行う
	useBillBoard_ = true;

	// ビルボード設定
	billBoardName_ = kBillBoardNameIndexYAxis;

	// 行列更新
	UpdateMatrix(Matrix4x4::MakeIdentity4x4());

	// 死亡フラグ
	isDead_ = false;
}

void GoalParticle::Update(const Matrix4x4& billBoardMatrix)
{
	// 速度によって移動させる
	transform_.translate += velocity_;

	if(transform_.scale.x > 0.0f){
		transform_.scale.x -= 0.01f;
		transform_.scale.y -= 0.01f;
	}
	else {
		transform_.scale.x = 0.0f;
		transform_.scale.y = 0.0f;
	}

	// 基底クラスの更新
	IParticle::Update(billBoardMatrix);
}
