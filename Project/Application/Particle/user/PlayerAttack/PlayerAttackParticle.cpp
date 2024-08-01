#include "PlayerAttackParticle.h"
#include "../../../../Engine/Math/RandomEngine.h"
#include "../../../../Engine/Math/Ease.h"
#include <numbers>

void PlayerAttackParticle::Initialize(ParticleDesc* particleDesc)
{
	// ランダムエンジン生成
	std::random_device seedGenerator;
	std::mt19937 randomEngine(seedGenerator());

	// サイズのランダム変数を生成
	float min = 0.0f;
	float max = 2.0f;
	std::uniform_real_distribution<float> distScale(min, max);
	int scaleNumber = static_cast<int>(distScale(randomEngine));
	float scale = 0.0f;
	if (scaleNumber == 0) {
		scale = 1.0f;
	}
	else if (scaleNumber == 1) {
		scale = 0.5f;
	}
	else {
		scale = 0.1f;
	}

	// 回転のランダム変数を生成
	float r = static_cast<float>(std::numbers::pi) * 2.0f;
	float minRotate = -r;
	float maxRotate = r;
	std::uniform_real_distribution<float> distRotate(minRotate, maxRotate);
	float rotate = distRotate(randomEngine);

	// 位置のランダム変数を生成
	Vector3 positionMax = { particleDesc->position.x + 1.0f,
							particleDesc->position.y + 1.0f,
							particleDesc->position.z + 1.0f };
	Vector3 positionMin = { particleDesc->position.x - 1.0f,
							particleDesc->position.y - 1.0f,
							particleDesc->position.z - 1.0f };
	std::uniform_real_distribution<float> randomPostionX(positionMin.x, positionMax.x);
	std::uniform_real_distribution<float> randomPostionY(positionMin.y, positionMax.y);
	std::uniform_real_distribution<float> randomPostionZ(positionMin.z, positionMax.z);



	// 速度のランダム変数を取得
	float minVelocity = 0.01f;
	float maxVelocity = .0025f;
	std::uniform_real_distribution<float> randomVelocity(minVelocity, maxVelocity);

	// 生存時間のランダム変数を取得
	std::uniform_real_distribution<float> distTime(0.25f, 0.5f);

	// 引数情報取得
	transform_.translate.x = randomPostionX(randomEngine);							 // 位置
	transform_.translate.y = randomPostionY(randomEngine);							 // 位置
	transform_.translate.z = randomPostionZ(randomEngine);							 // 位置
	transform_.rotate.z = rotate;												 // 回転を保持
	transform_.scale = { scale, scale, 1.0f };								 // 大きさ
	velocity_ = particleDesc->velocity * randomVelocity(randomEngine); // 速度

	// デフォルト大きさ値を取得
	defaultScale_ = transform_.scale;

	// デフォルト大きさ値を取得
	defaultVelocity_ = velocity_;

	// ワールド行列生成
	worldMatrix_ = Matrix4x4::MakeIdentity4x4();

	// 色設定
	color_ = { 1.0f,1.0f,1.0f, 1.0f };

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

void PlayerAttackParticle::Update(const Matrix4x4& billBoardMatrix)
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
		// y軸の速度は少しずつ下げる
		velocity_.y -= 0.0098f;
	}

	Vector3 s = Ease::Easing(Ease::EaseName::EaseOutQuad, defaultScale_, { 0.0f, 0.0f, 0.0f }, (currentTime_ / lifeTime_));
	transform_.scale = s;

	float r = Ease::Easing(Ease::EaseName::EaseOutQuad, 0.0f, 0.0f, (currentTime_ / lifeTime_));
	float g = Ease::Easing(Ease::EaseName::EaseOutQuad, 1.0f, 1.0f, (currentTime_ / lifeTime_));
	float b = Ease::Easing(Ease::EaseName::EaseOutQuad, 0.55f, 0.0f, (currentTime_ / lifeTime_));
	float a = Ease::Easing(Ease::EaseName::EaseOutQuad, 1.0f, 0.0f, (currentTime_ / lifeTime_));
	color_ = { r, g, b, a };

	transform_.rotate.z += 0.1f;

	// 基底クラス更新
	IParticle::Update(billBoardMatrix);
}
