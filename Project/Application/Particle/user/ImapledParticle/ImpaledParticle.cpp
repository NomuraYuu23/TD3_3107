#include "ImpaledParticle.h"
#include "../../../../Engine/Math/RandomEngine.h"
#include "../../../../Engine/Math/Ease.h"
#include <numbers>

void ImpaledParticle::Initialize(ParticleDesc* particleDesc)
{
	// ランダムエンジン生成
	std::random_device seedGenerator;
	std::mt19937 randomEngine(seedGenerator());

	// 生成座標を取得
	transform_.translate.x = particleDesc->position.x; // X軸
	transform_.translate.y = particleDesc->position.y; // Y軸
	transform_.translate.z = particleDesc->position.z; // Z軸

	// 生成直後の座標を格納
	prevPos_ = transform_.translate;

	// 生成サイズをランダムに取得
	std::uniform_real_distribution<float> randomSize(particleDesc->size.x, particleDesc->size.y);
	float size = randomSize(randomEngine);
	// 取得したサイズを設定
	transform_.scale.x = size;
	transform_.scale.y = size;
	transform_.scale.z = 1.0f;

	// 生成直後のサイズを格納
	prevScale_ = transform_.scale;

	// ランダムの角度を求める
	std::uniform_real_distribution<float> randomRotateDegree(-25.0f, 25.0f);
	float degree = randomRotateDegree(randomEngine);
	// ラジアンに変換する
	float radian = degree * (static_cast<float>(std::numbers::pi / 180.0f));
	// 角度を求める
	float cosAngle = std::cos(radian);
	float sinAngle = std::sin(radian);

	// 回転された速度ベクトルを求める
	velocity_.x = particleDesc->velocity.x * cosAngle - particleDesc->velocity.y * sinAngle;
	velocity_.y = particleDesc->velocity.x * sinAngle + particleDesc->velocity.y * cosAngle;

	velocity_.z = particleDesc->velocity.z;

	// どの程度移動するかをランダムに求める
	std::uniform_real_distribution<float> randomVelocityMagnification(7.5f, 10.0f);
	velocity_ *= randomVelocityMagnification(randomEngine); // スカラー倍

	// 処理後の座標を求める
	postPos_ = transform_.translate + velocity_;

	// ワールド行列生成
	worldMatrix_ = Matrix4x4::MakeIdentity4x4();

	// ランダムな色を取得
	std::uniform_real_distribution<float> distColor(0.5f, 1.0f);
	float color = distColor(randomEngine);
	// 色を設定
	color_ = { color, color, color, 1.0f };

	// 生成時間をランダムに取得
	std::uniform_real_distribution<float> distTime(0.75f, 1.25f);
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

void ImpaledParticle::Update(const Matrix4x4& billBoardMatrix)
{
	// イージングによって各々を動かす
	transform_.translate = Ease::Easing(Ease::EaseName::EaseOutQuad, prevPos_, postPos_, (currentTime_ / lifeTime_));
	transform_.scale = Ease::Easing(Ease::EaseName::EaseOutQuad, prevScale_, {0.0f, 00.f, 0.0f}, (currentTime_ / lifeTime_));

	// 基底クラスの更新
	IParticle::Update(billBoardMatrix);
}
