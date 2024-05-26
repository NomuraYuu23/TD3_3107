#include "SpearJumpParticle.h"
#include "../../../../Engine/Math/RandomEngine.h"

void SpearJumpParticle::Initialize(ParticleDesc* particleDesc)
{
	// ランダムエンジン生成
	std::random_device seedGenerator;
	std::mt19937 randomEngine(seedGenerator());

	// サイズのランダム変数を生成
	float minSize = 0.75f;
	float maxSize = 1.25f;
	std::uniform_real_distribution<float> distScale(minSize, maxSize);
	float scale = distScale(randomEngine);

	// 位置のランダム変数を生成
	Vector3 positionMax = { particleDesc->position.x + 1.5f,
							particleDesc->position.y,
							particleDesc->position.z };
	Vector3 positionMin = { particleDesc->position.x - 1.5f,
							particleDesc->position.y - 0.25f,
							particleDesc->position.z  };
	std::uniform_real_distribution<float> randomPostionX(positionMin.x, positionMax.x);
	std::uniform_real_distribution<float> randomPostionY(positionMin.y, positionMax.y);

	// 速度のランダム変数を取得
	float minVelocity = 0.35f;
	float maxVelocity = 0.45f;
	std::uniform_real_distribution<float> randomVelocity(minVelocity, maxVelocity);

	// X方向の速度のランダム変数を取得
	float minXVelocity = -0.15f;
	float maxXVelocity = 0.15f;
	std::uniform_real_distribution<float> randomXVelocity(minXVelocity, minXVelocity);


	// 生存時間のランダム変数を取得
	std::uniform_real_distribution<float> distTime(0.25f, 0.5f);

	// 引数情報取得
	transform_.translate.x = randomPostionX(randomEngine);				// 位置
	transform_.translate.y = randomPostionY(randomEngine);				// 位置
	transform_.translate.z = particleDesc->position.z;					// 位置											 // 回転を保持
	transform_.scale	   = { scale, scale, 1.0f };					// 大きさ
	velocity_ = particleDesc->velocity * randomVelocity(randomEngine);  // 速度
	velocity_.x = randomXVelocity(randomEngine);

	// ワールド行列生成
	worldMatrix_ = Matrix4x4::MakeIdentity4x4();

	// 色設定
	color_ = { 0.0f,1.0f,0.1f, .5f };

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

void SpearJumpParticle::Update(const Matrix4x4& billBoardMatrix)
{
	// 速度で移動させる
	transform_.translate += velocity_;

	// 基底クラスの更新
	IParticle::Update(billBoardMatrix);
}
