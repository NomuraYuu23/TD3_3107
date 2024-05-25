#include "SpearCatchParticle.h"
#include "../../../../Engine/Math/RandomEngine.h"

void SpearCatchParticle::Initialize(ParticleDesc* particleDesc)
{
	// ランダムエンジン生成
	std::random_device seedGenerator;
	std::mt19937 randomEngine(seedGenerator());

	// 位置のランダム変数を生成
	Vector3 positionMax = { particleDesc->position.x + 1.5f,
							particleDesc->position.y + 1.5f,
							particleDesc->position.z + 1.5f };
	Vector3 positionMin = { particleDesc->position.x - 1.5f,
							particleDesc->position.y - 1.5f,
							particleDesc->position.z - 1.5f };
	std::uniform_real_distribution<float> randomPostionX(positionMin.x, positionMax.x);
	std::uniform_real_distribution<float> randomPostionY(positionMin.y, positionMax.y);
	std::uniform_real_distribution<float> randomPostionZ(positionMin.z, positionMax.z);

	// サイズのランダム変数を生成
	Vector3 minScale = { particleDesc->size.x / 1.5f,
						 particleDesc->size.y / 1.5f,
						 particleDesc->size.z / 1.5f };
	Vector3 maxScale = { particleDesc->size.x * 1.5f,
						 particleDesc->size.y * 1.5f,
						 particleDesc->size.z * 1.5f };
	std::uniform_real_distribution<float> distScale(minScale.x, maxScale.x);
	float scale = distScale(randomEngine);

	// 速度のランダム変数を取得
	float minVelocity = 1.0f;
	float maxVelocity = 1.25f;
	std::uniform_real_distribution<float> randomVelocity(minVelocity, maxVelocity);

	// 生存時間のランダム変数を取得
	std::uniform_real_distribution<float> distTime(0.2f, 0.3f);

	// 引数情報取得
	transform_ .translate.x = randomPostionX(randomEngine);							 // 位置
	transform_ .translate.y = randomPostionY(randomEngine);							 // 位置
	transform_ .translate.z = randomPostionZ(randomEngine);							 // 位置
	transform_.scale		= { scale, scale, scale };								 // 大きさ
	velocity_				= particleDesc->velocity * randomVelocity(randomEngine); // 速度
	
	// デフォルト大きさ値を取得
	defaultVelocity_ = velocity_;

	// ワールド行列生成
	worldMatrix_ = Matrix4x4::MakeIdentity4x4();

	// 色設定
	color_ = { 1.0f,1.0f,1.0f, .5f };

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

void SpearCatchParticle::Update(const Matrix4x4& billBoardMatrix)
{
	// 速度ベクトルにしたがって粒子を移動させる
	transform_.translate += velocity_;

	if (defaultVelocity_.x > 0.0f) {
		if (velocity_.x > 0.0f) {
			velocity_.x -= 0.01f;
		}
		else {
			velocity_.x = 0.0f;
		}
	}
	else {
		if (velocity_.x < 0.0f) {
			velocity_.x += 0.01f;
		}
		else {
			velocity_.x = 0.0f;
		}
	}
	
	if (defaultVelocity_.y > 0.0f) {
		if (velocity_.y > 0.0f) {
			velocity_.y -= 0.01f;
		}
		else {
			velocity_.y = 0.0f;
		}
	}
	else {
		if (velocity_.y < 0.0f) {
			velocity_.y -= 0.01f;
		}
		else {
			velocity_.y = 0.0f;
		}
	}

	if (transform_.scale.x > 0.0f) {
		transform_.scale.x -= 0.1f;
		transform_.scale.y -= 0.1f;
	}
	else {
		transform_.scale = { 0.0f, 0.0f, 1.0f };
	}

	// 基底クラス更新
	IParticle::Update(billBoardMatrix);
}
