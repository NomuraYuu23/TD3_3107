#include "CheckPointObject.h"
#include "CheckPointManager.h"
#include "../../ObjectList.h"
#include "../../../Collider2D/CollisionConfig2D.h"
#include "../../../Engine/Math/DeltaTime.h"

CheckPointObject::~CheckPointObject()
{

	passedMaterial_.reset(nullptr);
	passedLocalMatrixManager_.reset(nullptr);

}

void CheckPointObject::Initialize(Model* model)
{
	// 基底クラスの初期化
	IObject::Initialize(model);
	// コライダー用の座標・スケール
	position2D_ = { worldtransform_.transform_.translate.x,worldtransform_.transform_.translate.y };
	scale2D_ = { worldtransform_.transform_.scale.x, worldtransform_.transform_.scale.y };
	scale2D_ = { 4.0f,8.0f };
	// コライダーの初期化
	boxCollider_.Initialize(position2D_, scale2D_.x, scale2D_.y, 0.0f, this);
	boxCollider_.SetCollisionAttribute(kCollisionAttributeGoalObject);
	boxCollider_.SetCollisionMask(kCollisionAttributeEnemy);

	// ライティング有効
	enableLighting_ = EnableLighting::HalfLambert;
	material_->SetEnableLighting(enableLighting_);
}

void CheckPointObject::Update()
{
	// 演出はデバッグ以外でのみ行う
#ifdef _RELEASE

	// 通過している場合
	if (isPassed_) {

		passedModelTransform_.transform_.scale = Ease::Easing(Ease::EaseName::EaseOutBack, { 1.0f, 0.0f, 1.0f }, { 1.0f, 0.75f, 1.0f }, (currentTime_ / stagingTime_));
		worldtransform_.transform_.scale = Ease::Easing(Ease::EaseName::EaseOutQuad, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f }, (currentTime_ / stagingTime_));
		float a = Ease::Easing(Ease::EaseName::EaseOutQuad, 1.0f, 0.0f, (currentTime_ / stagingTime_));
		material_->SetColor({ 1.0f, 1.0f, 1.0f, a });

		if (currentTime_ < stagingTime_) {
			currentTime_ += kDeltaTime_;
		}
		else {
			// 演出時間固定
			currentTime_ = stagingTime_;
		}
	}

#endif // !_DEBUG

	// 座標は合わせ続ける
	passedModelTransform_.transform_.translate = worldtransform_.transform_.translate;
	passedModelTransform_.UpdateMatrix();

	// 基底クラスの更新
	IObject::Update();
	// コライダー
	BoxColliderUpdate();
}

void CheckPointObject::Draw(const BaseCamera& camera)
{
	// 現在の演出時間が全体演出時間を超過していなければ
	if (currentTime_ < stagingTime_) {
		ModelDraw::AnimObjectDesc desc;
		desc.camera = &const_cast<BaseCamera&>(camera);
		desc.localMatrixManager = localMatrixManager_.get();
		desc.material = material_.get();
		desc.model = model_;
		desc.worldTransform = &worldtransform_;
		ModelDraw::AnimObjectDraw(desc);
	}
	
	// 通過していた場合
	if (isPassed_) {
		ModelDraw::AnimObjectDesc desc;
		desc.camera = &const_cast<BaseCamera&>(camera);
		desc.localMatrixManager = passedLocalMatrixManager_.get();
		desc.material = passedMaterial_.get();
		desc.model = passedModel_;
		desc.worldTransform = &passedModelTransform_;
		ModelDraw::AnimObjectDraw(desc);
	}
}

void CheckPointObject::ImGuiDraw()
{
}

void CheckPointObject::OnCollision(ColliderParentObject2D target)
{
	// 衝突時に書き換えの処理呼び出し（これより後の値だった場合書き換えない
	checkPointManager_->CheckPointJudge(worldtransform_.GetWorldPosition(), checkNum_);

#ifdef _RELEASE

	// 通過した場合再生とする
	if (!isPassed_) {
		// 通過
		isPassed_ = true;

		// 生成座標、および発生レンジ設定
		EulerTransform t = worldtransform_.transform_;
		t.translate.z -= 0.5f;
		t.scale = { 1.5f, 3.0f };

		// 煙パーティクル
		EmitterDesc desc;
		desc.transform = &t;
		desc.instanceCount = 3;
		desc.frequency = 0.15f;
		desc.lifeTime = stagingTime_;
		desc.velocity = { 0.1f, 0.1f, 0.0f };
		desc.particleModelNum = kSmoke;
		desc.paeticleName = kSmokePaticle;

		ParticleManager::GetInstance()->MakeEmitter(&desc, 0);

		// チェックポイント通過音を鳴らす
		checkPointManager_->GetGameAudioManager()->PlayWave(kPassCheckPoint);
	}

#endif // !_DEBUG
}

void CheckPointObject::Setting(const CheckPointData& data)
{
	// 座標設定
	worldtransform_.transform_.translate = data.position;
	// 番号設定
	checkNum_ = data.checkPointNumber;

	// 更新
	worldtransform_.UpdateMatrix();
}

void CheckPointObject::SetPassedModel(Model* model)
{
	// 通過後モデルを設定
	passedModel_ = model;

	// 通過マテリアル設定
	passedMaterial_.reset(Material::Create());
	passedMaterial_->SetEnableLighting(enableLighting_);
	passedMaterial_->SetShininess(shininess_);

	// トランスフォーム初期化
	passedModelTransform_.Initialize(model_->GetRootNode());
	// 拡大率初期化
	passedModelTransform_.transform_.scale = { 0.0f, 0.0f, 0.0f };

	// ローカル行列マネージャー
	passedLocalMatrixManager_ = std::make_unique<LocalMatrixManager>();
	passedLocalMatrixManager_->Initialize(model_->GetRootNode());
}
