#include "EditorScene.h"

EditorScene::~EditorScene()
{
}

void EditorScene::Initialize()
{
	IScene::Initialize();

	ModelCreate();
	TextureLoad();

	//// サンプルobj
	//sampleObj_ = std::make_unique<SampleObject>();
	//sampleObj_->Initialize(sampleObjModel_.get());
		// ビュープロジェクション
	TransformStructure baseCameraTransform = {
		1.0f, 1.0f, 1.0f,
		0.58f,0.0f,0.0f,
		0.0f, 23.0f, -100.0f };
	camera_.SetTransform(baseCameraTransform);
	isDebugCameraActive_ = false;

	// スカイドーム
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(skydomeModel_.get());

#pragma region 光源
	// 平行光源
	directionalLight_ = std::make_unique<DirectionalLight>();
	directionalLight_->Initialize();
	// 点光源
	pointLightManager_ = std::make_unique<PointLightManager>();
	pointLightManager_->Initialize();
	for (size_t i = 0; i < pointLightDatas_.size(); ++i) {
		pointLightDatas_[i].color = { 1.0f,1.0f,1.0f,1.0f };
		pointLightDatas_[i].position = { 0.0f, -1.0f, 0.0f };
		pointLightDatas_[i].intencity = 1.0f;
		pointLightDatas_[i].radius = 10.0f;
		pointLightDatas_[i].decay = 10.0f;
		pointLightDatas_[i].used = false;
	}

	spotLightManager_ = std::make_unique<SpotLightManager>();
	spotLightManager_->Initialize();
	for (size_t i = 0; i < spotLightDatas_.size(); ++i) {
		spotLightDatas_[i].color = { 1.0f,1.0f,1.0f,1.0f };
		spotLightDatas_[i].position = { 0.0f, -1.0f, 0.0f };
		spotLightDatas_[i].intencity = 1.0f;
		spotLightDatas_[i].direction = { 0.0f, -1.0f, 0.0f }; // ライトの方向
		spotLightDatas_[i].distance = 10.0f; // ライトの届く距離
		spotLightDatas_[i].decay = 2.0f; // 減衰率
		spotLightDatas_[i].cosAngle = 2.0f; // スポットライトの余弦
		spotLightDatas_[i].cosFalloffStart = 1.0f; // フォールオフ開始位置
		spotLightDatas_[i].used = false; // 使用している
	}
#pragma endregion

}

void EditorScene::Update()
{
	// デバッグカメラ
	DebugCameraUpdate();

	skydome_->Update();

}

void EditorScene::Draw()
{
	//ゲームの処理 

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(dxCommon_->GetCommadList());

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	renderTargetTexture_->ClearDepthBuffer();


#pragma endregion

	Model::PreDraw(dxCommon_->GetCommadList(), pointLightManager_.get(), spotLightManager_.get(), directionalLight_.get());

	//3Dオブジェクトはここ

	// スカイドーム
	skydome_->Draw(camera_);

#ifdef _DEBUG

	// デバッグ描画
	//colliderDebugDraw_->Draw(camera_);

#endif // _DEBUG

	Model::PostDraw();


#pragma region 大量のオブジェクト描画

	Model::PreManyModelsDraw(dxCommon_->GetCommadList(), pointLightManager_.get(), spotLightManager_.get(), directionalLight_.get());



	Model::PostDraw();

#pragma endregion

#pragma region 線描画
	DrawLine::PreDraw(dxCommon_->GetCommadList());



	DrawLine::PostDraw();

#pragma endregion

#pragma region パーティクル描画
	Model::PreParticleDraw(dxCommon_->GetCommadList(), camera_.GetViewProjectionMatrix());

	//光源
	directionalLight_->Draw(dxCommon_->GetCommadList(), 6);

	// パーティクルはここ
	//particleManager_->Draw();

	Model::PostDraw();

#pragma endregion

#ifdef _DEBUG
#pragma region コライダー2d描画


#pragma endregion
#endif // DEBUG_


#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(dxCommon_->GetCommadList());


	//背景
	//前景スプライト描画


	// 前景スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion

}

void EditorScene::ImguiDraw()
{
}

void EditorScene::DebugCameraUpdate()
{
#ifdef _DEBUG
	if (input_->TriggerKey(DIK_RETURN)) {
		if (isDebugCameraActive_) {
			isDebugCameraActive_ = false;
		}
		else {
			isDebugCameraActive_ = true;
		}
	}

#endif
	// カメラの処理
	if (isDebugCameraActive_) {
		// デバッグカメラの更新
		debugCamera_->Update();
		// デバッグカメラのビュー行列をコピー
		camera_ = static_cast<BaseCamera>(*debugCamera_.get());
		// ビュー行列の転送
		camera_.Update();
	}
	else {
		// デバッグカメラの更新
		debugCamera_->Update();
		// デバッグカメラのビュー行列をコピー
		camera_ = static_cast<BaseCamera>(*debugCamera_.get());
		// ビュー行列の転送
		camera_.Update();
	}

}

void EditorScene::ModelCreate()
{
	// スカイドーム
	skydomeModel_.reset(Model::Create("Resources/Model/Skydome/", "skydome.obj", dxCommon_, textureHandleManager_.get()));

}

void EditorScene::TextureLoad()
{

}
