#include "SpearJumpParticle.h"
#include "../../../../Engine/Math/RandomEngine.h"
#include <numbers>

void SpearJumpParticle::Initialize(ParticleDesc* particleDesc)
{
	// ランダムエンジン生成
	std::random_device seedGenerator;
	std::mt19937 randomEngine(seedGenerator());

	// 生成範囲をランダムに取得
	std::uniform_real_distribution<float> randomRangeX(-particleDesc->size.x, particleDesc->size.x);
	std::uniform_real_distribution<float> randomRangeY(-particleDesc->size.y, particleDesc->size.y);

	// 生成座標をランダムに取得
	transform_.translate.x = particleDesc->position.x + randomRangeX(randomEngine); // X軸
	transform_.translate.y = particleDesc->position.y + randomRangeY(randomEngine); // Y軸
	transform_.translate.z = particleDesc->position.z;								// Z軸

	// 生成サイズをランダムに取得
	std::uniform_real_distribution<float> randomSize(0.75f, 1.5f);
	float size = randomSize(randomEngine);
	// 取得したサイズを設定
	transform_.scale.x = size;
	transform_.scale.y = size;

	// 生成時の角度をランダムに取得
	std::uniform_real_distribution<float> randomDirection(-static_cast<float>(std::numbers::pi * 2.0f), static_cast<float>(std::numbers::pi * 2.0f));
	float rotate = randomDirection(randomEngine);
	transform_.rotate.z = rotate;

	// 回転方向を取得
	std::uniform_real_distribution<float> randomRotateDirection(0.0f, 1.0f);
	int32_t isRight = static_cast<int32_t>(randomRotateDirection(randomEngine));
	isRight_ = isRight;

	// 生存時間のランダム変数を取得
	std::uniform_real_distribution<float> distTime(0.5f, 1.0f);

	// ワールド行列生成
	worldMatrix_ = Matrix4x4::MakeIdentity4x4();

	// 色設定
	color_ = { 1.0f, 1.0f, 1.0f, 1.0f };

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
	// 回転方向フラグで回転方向を決める
	if (isRight_) {
		transform_.rotate.z += 0.1f;
	}
	else {
		transform_.rotate.z -= 0.1f;
	}

	// 基底クラスの更新
	IParticle::Update(billBoardMatrix);
}
