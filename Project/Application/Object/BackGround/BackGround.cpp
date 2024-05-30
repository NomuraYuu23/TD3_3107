#include "BackGround.h"
#include "../../../Engine/3D/ModelDraw.h"
#include "../../imgui/imgui.h"
#include "../../../Engine/base/TextureManager.h"
#include "../../../Engine/Math/Math.h"

void BackGround::Initialize(Model* model)
{
	// 基底クラスの初期化
	IObject::Initialize(model);

	// トランスフォーム初期化
	worldtransform_.transform_.translate = { 150.0f, -0.0f, 300.0f };
	worldtransform_.transform_.scale = { 3.0f, 3.0f, 3.0f };

	// トランスフォーム初期化
	transform2_.Initialize(model_->GetRootNode());
	transform2_.transform_.translate = { 450.0f, -0.0f, 300.0f };
	transform2_.transform_.scale = { 3.0f, 3.0f, 3.0f };
	transform2_.UpdateMatrix();
	// トランスフォーム初期化
	transform3_.Initialize(model_->GetRootNode());
	transform3_.transform_.translate = { 750.0f, -0.0f, 300.0f };
	transform3_.transform_.scale = { 3.0f, 3.0f, 3.0f };
	transform3_.UpdateMatrix();

	// ライティング有効
	enableLighting_ = EnableLighting::HalfLambert;
	material_->SetEnableLighting(enableLighting_);
}

void BackGround::Update()
{
	// チュートリアルモデルが描画されていればチュートリアル映像の再生を行う
	if (speartutorialPlaneModel_ != nullptr) {
		
		int32_t spearTexIndex = static_cast<int32_t>(Math::Lerp(0, 11, spearCurrentTime_ / spearStagingTime_));
		int32_t enemyTexIndex = static_cast<int32_t>(Math::Lerp(0, 10, enemyCurrentTime_ / enemyStagingTime_));

		// テクスチャ変更
		speartutorialPlaneModel_->SetTextureHandle(spearTutorialTexHandles_[spearTexIndex], 0);
		enemytutorialPlaneModel_->SetTextureHandle(enemyTutorialTexHandles_[enemyTexIndex], 0);

		// 現在時間に更新秒数加算
		spearCurrentTime_ += updateTime_;
		enemyCurrentTime_ += updateTime_;

		if (spearCurrentTime_ > spearStagingTime_) {
			spearCurrentTime_ = 0.0f;
		}

		if (enemyCurrentTime_ > enemyStagingTime_) {
			enemyCurrentTime_ = 0.0f;
		}

		// トランスフォームの更新
		spearTutorialTransform_.UpdateMatrix();
		enemyTutorialTransform_.UpdateMatrix();
	}

	// 基底クラスの更新
	IObject::Update();
}

void BackGround::Draw(const BaseCamera& camera)
{
	ModelDraw::AnimObjectDesc desc;
	desc.camera = &const_cast<BaseCamera&>(camera);
	desc.localMatrixManager = localMatrixManager_.get();
	desc.material = material_.get();
	desc.model = model_;
	desc.worldTransform = &worldtransform_;
	ModelDraw::AnimObjectDraw(desc);

	desc.worldTransform = &transform2_;
	ModelDraw::AnimObjectDraw(desc);
	desc.worldTransform = &transform3_;
	ModelDraw::AnimObjectDraw(desc);

	// チュートリアルモデルが描画されていればチュートリアル平面の描画を行う
	if (speartutorialPlaneModel_ != nullptr) {
		spearTutorialTransform_.transform_.rotate.x = 0.3f;
		enemyTutorialTransform_.transform_.rotate.x = 0.3f;
		ModelDraw::NormalObjectDesc desc;
		desc.camera = &const_cast<BaseCamera&>(camera);
		desc.material = tutorialPlaneMaterial_.get();
		desc.model = speartutorialPlaneModel_;
		desc.worldTransform = &spearTutorialTransform_;
		ModelDraw::NormalObjectDraw(desc);

		desc.model = enemytutorialPlaneModel_;
		desc.worldTransform = &enemyTutorialTransform_;
		ModelDraw::NormalObjectDraw(desc);
	}
}

void BackGround::ImGuiDraw()
{
	ImGui::Begin("BackGround");
	ImGui::DragFloat3("transform", &worldtransform_.transform_.translate.x);

	ImGui::DragFloat3("spearTutorial_scale", &spearTutorialTransform_.transform_.scale.x);
	ImGui::DragFloat3("spearTutorial_rotate", &spearTutorialTransform_.transform_.rotate.x);
	ImGui::DragFloat3("spearTutorial_translate", &spearTutorialTransform_.transform_.translate.x);
	ImGui::DragFloat3("enemyTutorial_scale", &enemyTutorialTransform_.transform_.scale.x);
	ImGui::DragFloat3("enemyTutorial_translate", &enemyTutorialTransform_.transform_.translate.x);

	ImGui::End();
}

void BackGround::OnCollision(ColliderParentObject2D target)
{
}

Box BackGround::GetBoxCollider()
{
	return Box();
}

Vector2 BackGround::GetColliderPosition()
{
	return Vector2();
}

Vector2 BackGround::GetColliderSize()
{
	return Vector2();
}

void BackGround::SetTutorialPlaneModel(ITextureHandleManager* texHandleManager, Model* spearJump, Model* enemy)
{
	// インスタンスの取得
	dxCommon_ = DirectXCommon::GetInstance(); // DirectX汎用クラス
	texHandleManager_ = texHandleManager;	  // テクスチャハンドルマネージャー

	// 引数からモデル取得
	speartutorialPlaneModel_ = spearJump;
	enemytutorialPlaneModel_ = enemy;

	// 槍ジャンプ用チュートリアル画像
	spearTutorialTexHandles_.insert({ 0, TextureManager::Load("Resources/UI/Tutorials/SpearJump/SpearJump_00.png", dxCommon_, texHandleManager_) });
	spearTutorialTexHandles_.insert({ 1, TextureManager::Load("Resources/UI/Tutorials/SpearJump/SpearJump_01.png", dxCommon_, texHandleManager_) });
	spearTutorialTexHandles_.insert({ 2, TextureManager::Load("Resources/UI/Tutorials/SpearJump/SpearJump_02.png", dxCommon_, texHandleManager_) });
	spearTutorialTexHandles_.insert({ 3, TextureManager::Load("Resources/UI/Tutorials/SpearJump/SpearJump_03.png", dxCommon_, texHandleManager_) });
	spearTutorialTexHandles_.insert({ 4, TextureManager::Load("Resources/UI/Tutorials/SpearJump/SpearJump_04.png", dxCommon_, texHandleManager_) });
	spearTutorialTexHandles_.insert({ 5, TextureManager::Load("Resources/UI/Tutorials/SpearJump/SpearJump_05.png", dxCommon_, texHandleManager_) });
	spearTutorialTexHandles_.insert({ 6, TextureManager::Load("Resources/UI/Tutorials/SpearJump/SpearJump_06.png", dxCommon_, texHandleManager_) });
	spearTutorialTexHandles_.insert({ 7, TextureManager::Load("Resources/UI/Tutorials/SpearJump/SpearJump_07.png", dxCommon_, texHandleManager_) });
	spearTutorialTexHandles_.insert({ 8, TextureManager::Load("Resources/UI/Tutorials/SpearJump/SpearJump_08.png", dxCommon_, texHandleManager_) });
	spearTutorialTexHandles_.insert({ 9, TextureManager::Load("Resources/UI/Tutorials/SpearJump/SpearJump_09.png", dxCommon_, texHandleManager_) });
	spearTutorialTexHandles_.insert({ 10, TextureManager::Load("Resources/UI/Tutorials/SpearJump/SpearJump_10.png", dxCommon_, texHandleManager_) });

	// 敵用チュートリアル画像
	enemyTutorialTexHandles_.insert({ 0, TextureManager::Load("Resources/UI/Tutorials/Enemy/Enemy_00.png", dxCommon_, texHandleManager_) });
	enemyTutorialTexHandles_.insert({ 1, TextureManager::Load("Resources/UI/Tutorials/Enemy/Enemy_01.png", dxCommon_, texHandleManager_) });
	enemyTutorialTexHandles_.insert({ 2, TextureManager::Load("Resources/UI/Tutorials/Enemy/Enemy_02.png", dxCommon_, texHandleManager_) });
	enemyTutorialTexHandles_.insert({ 3, TextureManager::Load("Resources/UI/Tutorials/Enemy/Enemy_03.png", dxCommon_, texHandleManager_) });
	enemyTutorialTexHandles_.insert({ 4, TextureManager::Load("Resources/UI/Tutorials/Enemy/Enemy_04.png", dxCommon_, texHandleManager_) });
	enemyTutorialTexHandles_.insert({ 5, TextureManager::Load("Resources/UI/Tutorials/Enemy/Enemy_05.png", dxCommon_, texHandleManager_) });
	enemyTutorialTexHandles_.insert({ 6, TextureManager::Load("Resources/UI/Tutorials/Enemy/Enemy_06.png", dxCommon_, texHandleManager_) });
	enemyTutorialTexHandles_.insert({ 7, TextureManager::Load("Resources/UI/Tutorials/Enemy/Enemy_07.png", dxCommon_, texHandleManager_) });
	enemyTutorialTexHandles_.insert({ 8, TextureManager::Load("Resources/UI/Tutorials/Enemy/Enemy_08.png", dxCommon_, texHandleManager_) });
	enemyTutorialTexHandles_.insert({ 9, TextureManager::Load("Resources/UI/Tutorials/Enemy/Enemy_09.png", dxCommon_, texHandleManager_) });

	// モデルのデフォルトテクスチャを変更
	speartutorialPlaneModel_->SetTextureHandle(spearTutorialTexHandles_[0], 0);
	enemytutorialPlaneModel_->SetTextureHandle(enemyTutorialTexHandles_[0], 0);

	// 演出時間を求める
	spearStagingTime_ = 0.5f * 11;
	enemyStagingTime_ = 0.5f * 10;

	// ワールドトランスフォームの初期化
	spearTutorialTransform_.Initialize(model_->GetRootNode());
	spearTutorialTransform_.transform_.translate = {90.0f, 14.5f, 15.0f };
	spearTutorialTransform_.transform_.scale = {12.5f, 12.5f, 1.0f };
	spearTutorialTransform_.UpdateMatrix();
	enemyTutorialTransform_.Initialize(model_->GetRootNode());
	enemyTutorialTransform_.transform_.translate = { 150.0f, 25.0f, 15.0f };
	enemyTutorialTransform_.transform_.scale = { 12.5f, 12.5f, 1.0f };
	enemyTutorialTransform_.UpdateMatrix();

	tutorialPlaneMaterial_.reset(Material::Create());
	tutorialPlaneMaterial_->SetEnableLighting(EnableLighting::None);
	tutorialPlaneMaterial_->SetShininess(shininess_);
}
