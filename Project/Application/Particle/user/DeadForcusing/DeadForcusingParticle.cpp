#include "DeadForcusingParticle.h"
#include "../../../../Engine/Math/RandomEngine.h"
#include "../../../../Engine/Math/Ease.h"

void DeadForcusingParticle::Initialize(ParticleDesc* particleDesc)
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

	// 線形補間前座標を取得
	prePos_ = transform_.translate;
	// 線形補間後座標を取得
	postPos_ = particleDesc->position;

	// 生成サイズをランダムに取得
	std::uniform_real_distribution<float> randomSize(2.5f, 3.5f);
	float size = randomSize(randomEngine);
	// 取得したサイズを設定
	transform_.scale.x = size;
	transform_.scale.y = size;

	// 速度をランダムに取得
	std::uniform_real_distribution<float> randomVecX(-particleDesc->velocity.x, particleDesc->velocity.x);
	std::uniform_real_distribution<float> randomVecY(-particleDesc->velocity.y, particleDesc->velocity.y);
	// ランダムな速度ベクトルを取得
	velocity_.x = randomVecX(randomEngine);
	velocity_.y = randomVecY(randomEngine);
	velocity_.z = 0.0f;

	// ワールド行列生成
	worldMatrix_ = Matrix4x4::MakeIdentity4x4();

	// ランダムな色を取得
	std::uniform_real_distribution<float> distColor(1.0f, 2.5f);
	float color = distColor(randomEngine);
	// 色を設定
	color_ = { color, color, color, 1.0f };

	// 生成時間をランダムに取得
	std::uniform_real_distribution<float> distTime(1.0f, 1.5f);
	// ランダムに粒子生存時間を取得
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

void DeadForcusingParticle::Update(const Matrix4x4& billBoardMatrix)
{
	// 速度ベクトルを0に近づけていく
	transform_.translate = Ease::Easing(Ease::EaseName::EaseOutQuad, prePos_, postPos_, (currentTime_ / lifeTime_));

	// 基底クラスの更新
	IParticle::Update(billBoardMatrix);
}
