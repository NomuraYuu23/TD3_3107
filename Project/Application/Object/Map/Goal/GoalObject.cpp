#include "GoalObject.h"
#include "../../ObjectList.h"
#include "../../../Collider2D/CollisionConfig2D.h"
#include "../../../Particle/EmitterName.h"

void GoalObject::Initialize(Model* model)
{
	// 基底クラスの初期化
	IObject::Initialize(model);
	// コライダー用の座標・スケール
	position2D_ = { worldtransform_.transform_.translate.x,worldtransform_.transform_.translate.y };
	scale2D_ = { worldtransform_.transform_.scale.x, worldtransform_.transform_.scale.y };
	scale2D_ = { 2.0f,2.0f };
	// コライダーの初期化
	boxCollider_.Initialize(position2D_, scale2D_.x, scale2D_.y, 0.0f, this);
	boxCollider_.SetCollisionAttribute(kCollisionAttributeGoalObject);
	boxCollider_.SetCollisionMask(kCollisionAttributeEnemy);

	// ライティング有効
	enableLighting_ = EnableLighting::HalfLambert;
	material_->SetEnableLighting(enableLighting_);

	// システム関係の初期化
	SystemInitialize();

#pragma region 調整項目クラス
	// 調整項目クラスのインスタンス取得
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	// グループ名設定
	const char* groupName = "Goal";
	// 指定した名前でグループ追加
	globalVariables->CreateGroup(groupName);

	// メンバ変数の調整したい項目をグローバル変数に追加

	std::string name = "";

	for (uint32_t i = 0; i < StageNumberManager::kStageMax; ++i) {
		if (i < 10) {
			name = "GoalPosition0" + std::to_string(i);
		}
		else {
			name = "GoalPosition" + std::to_string(i);
		}
		globalVariables->AddItem(groupName, name, goalPositions_[i]);	
	}

	ApplyGlobalVariables();

#pragma endregion

	// デバック以外の場合行う
	#ifndef _DEBUG

	// トランスフォーム生成
	emitTransform_ = std::make_unique<EulerTransform>();
	*emitTransform_ = worldtransform_.transform_;

	// ここでゴールパーティクルの再生を行う
	goalParticleDesc_.transform = emitTransform_.get();
	goalParticleDesc_.instanceCount = 3;
	goalParticleDesc_.frequency = 0.25f;
	goalParticleDesc_.lifeTime = 5.0f;
	goalParticleDesc_.particleModelNum = kCircle;
	goalParticleDesc_.paeticleName = kGoalParticle;
	goalParticleDesc_.velocity = { 0.0f, 1.0f, 0.0f };

	// 無限生成エミッタで生成し続ける
	ParticleManager::GetInstance()->MakeEmitter(&goalParticleDesc_, EmitterName::kInfiniteEmitter);

	#endif // !_DEBUG
}

void GoalObject::Update()
{

#ifdef _DEBUG
	ApplyGlobalVariables();
#endif // _DEBUG

	// デバック以外の場合行う
	#ifndef _DEBUG

	// 生成座標更新
	*emitTransform_ = worldtransform_.transform_;
	emitTransform_->translate.y += 1.5f;

	#endif // !_DEBUG

	// 基底クラスの更新
	IObject::Update();
	// コライダー
	BoxColliderUpdate();

}

void GoalObject::Draw(const BaseCamera& camera)
{
	ModelDraw::AnimObjectDesc desc;
	desc.camera = &const_cast<BaseCamera&>(camera);
	desc.localMatrixManager = localMatrixManager_.get();
	desc.material = material_.get();
	desc.model = model_;
	desc.worldTransform = &worldtransform_;
	ModelDraw::AnimObjectDraw(desc);
}

void GoalObject::ImGuiDraw()
{
	if (ImGui::TreeNode("GoalObject")) {
		ImGui::SeparatorText("GoalObject");
		int goalFlag = isGoal_;
		ImGui::InputInt("IsGoal", &goalFlag);
		ImGui::TreePop();
	}

	ImGui::Text("\n");
}

void GoalObject::OnCollision(ColliderParentObject2D target)
{
	if (std::holds_alternative<Player*>(target)) {
		isGoal_ = true;
	}
}

void GoalObject::SystemInitialize()
{
	isGoal_ = false;
}

void GoalObject::ApplyGlobalVariables()
{

	// 調整項目クラスのインスタンス取得
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	// グループ名の設定
	const char* groupName = "Goal";

	std::string name = "";

	for (uint32_t i = 0; i < StageNumberManager::kStageMax; ++i) {

		if (i < 10) {
			name = "GoalPosition0" + std::to_string(i);
		}
		else {
			name = "GoalPosition" + std::to_string(i);
		}
		goalPositions_[i] = globalVariables->GetVector3Value(groupName, name);
	}

}

void GoalObject::SetPosition()
{

	worldtransform_.transform_.translate = goalPositions_[StageNumberManager::stageNum_];

}
