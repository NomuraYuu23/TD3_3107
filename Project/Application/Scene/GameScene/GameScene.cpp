#include "GameScene.h"
#include "../../../Engine/base/WinApp.h"
#include "../../../Engine/base/TextureManager.h"
#include "../../../Engine/2D/ImguiManager.h"
#include "../../../Engine/base/D3DResourceLeakChecker.h"
#include "../../../Engine/GlobalVariables/GlobalVariables.h"
#include "../../Particle/EmitterName.h"
#include "../../../Engine/Math/DeltaTime.h"
#include "../../../Engine/base/WindowSprite.h"

GameScene::~GameScene()
{

	if (stopAudio_) {
		for (uint32_t i = 0; i < audioManager_->kMaxPlayingSoundData; ++i) {
			audioManager_->StopWave(i);
		}
	}

	particleManager_->Finalize();

}

/// <summary>
/// 初期化
/// </summary>
void GameScene::Initialize() {

	IScene::Initialize();

	ModelCreate();
	TextureLoad();

	// ビュープロジェクション
	EulerTransform baseCameraTransform = {
		1.0f, 1.0f, 1.0f,
		0.58f,0.0f,0.0f,
		0.0f, 23.0f, -100.0f };
	camera_.SetTransform(baseCameraTransform);

	//パーティクル
	particleManager_ = ParticleManager::GetInstance();
	std::array<Model*, ParticleModelIndex::kCountofParticleModelIndex> particleModel;
	particleModel[ParticleModelIndex::kUvChecker] = particleUvcheckerModel_.get();
	particleModel[ParticleModelIndex::kCircle] = particleCircleModel_.get();
	particleManager_->ModelCreate(particleModel);


	isDebugCameraActive_ = false;

	collisionManager_.reset(new CollisionManager);
	collisionManager_->Initialize();

	//UIマネージャー
	//uiManager_ = std::make_unique<UIManager>();
	//uiManager_->Initialize(uiTextureHandles_);

	// オーディオマネージャー
	audioManager_ = std::make_unique<GameAudioManager>();
	audioManager_->Initialize();
	//uiManager_->SetAudioManager(audioManager_.get());

	// スカイドーム
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(skydomeModel_.get());

	// サンプルobj
	sampleObj_ = std::make_unique<SampleObject>();
	sampleObj_->Initialize(sampleObjModel_.get());

	collision2DManager_ = std::make_unique<Collision2DManager>();
	collision2DManager_->Initialize();

	collision2DDebugDraw_ = std::make_unique<Collision2DDebugDraw>();
	collision2DDebugDraw_->Initialize(dxCommon_->GetDevice(), collision2DDebugDrawTextures_,
		GraphicsPipelineState::sRootSignature[GraphicsPipelineState::kPipelineStateIndexCollision2DDebugDraw].Get(),
		GraphicsPipelineState::sPipelineState[GraphicsPipelineState::kPipelineStateIndexCollision2DDebugDraw].Get());


	// プレイヤーの初期化
	// 武器の生成
	std::unique_ptr<Weapon> weapon = std::make_unique<Weapon>();
	// 初期化
	weapon->Initialize(weaponModel_.get());
	
	// デバッグ以外の場合行う
	#ifndef _DEBUG
	// リングモデルを渡す
	weapon->SetRingModel(ringModel_.get());
	#endif // !_DEBUG


	// 生成
	player_ = std::make_unique<Player>();
	// テクスチャの読み込み
	player_->arrowTexture_ = TextureManager::Load("Resources/GameObject/Image/arrow.png", dxCommon_, textureHandleManager_.get());
	player_->SetArrowModel(particleCircleModel_.get());
	// 武器の設定
	player_->SetWeapon(std::move(weapon));
	// 初期化
	player_->Initialize(playerModel_.get());
	// デバッグ以外の場合行う
	#ifndef _DEBUG
	// ポニーテール
	player_->SetPonyTail(ponyTailModel_.get());
	// オーディオマネージャーを渡す
	player_->gameAudioManager_ = audioManager_.get();
	#endif // !_DEBUG

	//player_->SetPonyTail(ponyTailModel_.get());
	player_->GetSlowEffect()->SetCamera(&camera_);

	// 更新
	//countTime_ = 0;

	// 敵管理クラス
	enemyManager_ = std::make_unique<EnemyManager>();
	enemyManager_->Initialize(enemyModel_.get());

	player_->SetEnemyManager(enemyManager_.get());
	player_->Update();

	bossEnemy_ = std::make_unique<PrevSmallBoss>();
	bossEnemy_->Initialize(enemyModel_.get());

	// マップ管理クラス
	mapManager_ = std::make_unique<MapManager>();
	mapManager_->blockTexture_ = TextureManager::Load("Resources/default/white2x2.png", DirectXCommon::GetInstance(), textureHandleManager_.get());
	mapManager_->Initialize(terrainModel_.get());

	// 背景用オブジェクト
	backGround_ = std::make_unique<BackGround>();
	backGround_->Initialize(backGroundModel_.get());

	// 定点カメラ（仮
	gameCamera_ = std::make_unique<GameBasicCamera>();
	gameCamera_->Initialize();

	// 追従カメラ（仮
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();
	followCamera_->SetPlayer(player_.get());


	// 矢印のUI
	arrowSprite_.reset(Sprite::Create(player_->arrowTexture_, { 100,100 }, { 1,1,1,1 }));
	arrowSprite_->SetAnchorPoint({ 0.5f,0.5f });
	arrowSprite_->SetSize({ arrowSprite_->GetSize().x / 6,arrowSprite_->GetSize().y / 6 });
	arrowSprite_->SetRotate(std::atan2f(player_->throwDirect_.y, player_->throwDirect_.x));

	/// ポストエフェクトの値初期化
	// ブルーム
	PostEffect* pe = PostEffect::GetInstance();
	pe->SetThreshold(0.05f);
	pe->SetKernelSize(5);
	pe->SetSigma(5.0f);

	FogManager* fm = FogManager::GetInstance();
	fm->SetColor({ 0.0f, 0.35f, 1.0f, 1.0f });
	fm->SetNear(50.0f);
	fm->SetRadius(2500.0f);

	// Jsonデータのクラス
#ifdef _DEBUG

	gameData_ = GameObjectData::GetInstance();
	gameData_->Initialize();

#endif // _DEBUG

}

/// <summary>
/// 更新処理
/// </summary>
void GameScene::Update() {

#ifdef _DEBUG
	ImguiDraw();

	if (input_->TriggerKey(DIK_L)) {
		requestSceneNo_ = kTitle;
	}


#endif
	if (input_->TriggerKey(DIK_R)) {
		this->Initialize();
	}

	if (input_->TriggerKey(DIK_L)) {
		requestSceneNo_ = kTitle;
	}

	if (requestSceneNo_ == kClear || requestSceneNo_ == kTitle || isBeingReset_) {
		resetScene_ = false;
		// BGM音量下げる
		if (isDecreasingVolume) {
			LowerVolumeBGM();
		}
		return;
	}

	// リスタート
	//if () {
	//	resetScene_ = true;
	//	isBeingReset_ = true;
	//	isDecreasingVolume = true;
	//}


	directionalLight_->Update(directionalLightData_);

	pointLightManager_->Update(pointLightDatas_);
	spotLightManager_->Update(spotLightDatas_);

	if (player_->GetEffectInfo().isStop) {
		player_->HitUpdate();
		// デバッグカメラ
		DebugCameraUpdate();
		return;
	}

	//Obj
	// マップ
	mapManager_->Update();
	// プレイヤー
	player_->Update();
	player_->DrawLinesMap(drawLine_);
	// 敵
	enemyManager_->Update();
	//bossEnemy_->Update();

	if (player_->isArrowUiDraw_) {
		arrowSprite_->SetIsInvisible(false);
	}
	else {
		arrowSprite_->SetIsInvisible(true);
	}

	arrowSprite_->SetPosition(player_->screenPos_);
	arrowSprite_->SetRotate(std::atan2f(-player_->throwDirect_.y, player_->throwDirect_.x));

	//arrowSprite_->Update();

	// 当たり判定の設定とチェック
	CollisionUpdate();

	// 影
	ShadowUpdate();

	// スカイドーム
	skydome_->Update();

	// 背景更新
	backGround_->Update();

	//uiManager_->Update();

	// デバッグカメラ
	DebugCameraUpdate();

	//パーティクル
	particleManager_->Update(camera_);

}

/// <summary>
/// 描画処理
/// </summary>
void GameScene::Draw() {

	//ゲームの処理 

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(dxCommon_->GetCommadList());

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	renderTargetTexture_->ClearDepthBuffer();


#pragma endregion

	ModelDraw::PreDrawDesc preDrawDesc;
	preDrawDesc.commandList = dxCommon_->GetCommadList();
	preDrawDesc.directionalLight = directionalLight_.get();
	preDrawDesc.fogManager = FogManager::GetInstance();
	preDrawDesc.pointLightManager = pointLightManager_.get();
	preDrawDesc.spotLightManager = spotLightManager_.get();

	ModelDraw::PreDraw(preDrawDesc);

	//3Dオブジェクトはここ

	// スカイドーム
	skydome_->Draw(camera_);

	// 背景
	backGround_->Draw(camera_);

	//Obj
	player_->Draw(camera_);
	//bossEnemy_->Draw(camera_);

	tmpTextures_.clear();
	tmpTextures_.push_back(blockTexture_);

	// ブロック用
	mapManager_->Draw(camera_);

	tmpTextures_.clear();
	tmpTextures_.push_back(enemyTexture_);

	// 敵
	enemyManager_->Draw(camera_, &tmpTextures_);

	// プレイヤーのリングは透過するため最後に描画
	player_->weapon_->RingDraw(camera_);

	ModelDraw::PostDraw();

#pragma endregion

#pragma region 線描画

	drawLine_->Draw(dxCommon_->GetCommadList(), camera_);

#pragma endregion

#pragma region パーティクル描画

	// パーティクルはここ
	particleManager_->Draw(camera_.GetViewProjectionMatrix(), dxCommon_->GetCommadList());

#pragma endregion

#ifdef _DEBUG
#pragma region コライダー2d描画

	collision2DDebugDraw_->Draw(dxCommon_->GetCommadList());

#pragma endregion
#endif // DEBUG_


#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(dxCommon_->GetCommadList());


	//背景
	//前景スプライト描画

	// UIマネージャー
	//uiManager_->Draw();
	arrowSprite_->Draw();

	// 前景スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
	if (player_->GetHitManager().IsHitEffectActive()) {
		PlayerHitManager::Effect instance = player_->GetEffectInfo();
		PostEffect::GetInstance()->SetRShift(instance.rShift);
		PostEffect::GetInstance()->SetGShift(instance.gShift);
		PostEffect::GetInstance()->SetBShift(instance.rShift);
		PostEffect::GetInstance()->SetTime(instance.time);
		PostEffect::GetInstance()->Execution(
			dxCommon_->GetCommadList(),
			renderTargetTexture_,
			PostEffect::kCommandIndexGlitchRGBShift);
		WindowSprite::GetInstance()->DrawUAV(PostEffect::GetInstance()->GetEditTextures(0)->GetUavHandleGPU());
	}
	if (player_->GetWeapon()->GetEffectSystem()->IsActive()) {
		//PostEffect::GetInstance()-
		PostEffect::ExecutionAdditionalDesc desc = {};
		desc.shockWaveManagers[0] = player_->GetWeapon()->GetEffectSystem()->GetShockWaveManager();
		PostEffect::GetInstance()->SetTime(3.0f);
		PostEffect::GetInstance()->Execution(
			dxCommon_->GetCommadList(),
			renderTargetTexture_,
			PostEffect::kCommandIndexShockWave,
			&desc);
		WindowSprite::GetInstance()->DrawUAV(PostEffect::GetInstance()->GetEditTextures(0)->GetUavHandleGPU());
	}
	if (player_->GetSlowEffect()->GetRunning()) {
		PostEffect::GetInstance()->SetColorPosition(player_->GetSlowEffect()->GetCenter());
		PostEffect::GetInstance()->SetColorSize(player_->GetSlowEffect()->GetSize());
		PostEffect::GetInstance()->SetColorLerpT(player_->GetSlowEffect()->GetColorT());
		PostEffect::GetInstance()->SetTime(3.0f);
		PostEffect::GetInstance()->Execution(
			dxCommon_->GetCommadList(),
			renderTargetTexture_,
			PostEffect::kCommandIndexGrayScale);
		WindowSprite::GetInstance()->DrawUAV(PostEffect::GetInstance()->GetEditTextures(0)->GetUavHandleGPU());
	}

	/*PostEffect::GetInstance()->Execution(
		dxCommon_->GetCommadList(),
		renderTargetTexture_,
		PostEffect::kCommandIndexBloom);
	WindowSprite::GetInstance()->DrawUAV(PostEffect::GetInstance()->GetEditTextures(0)->GetUavHandleGPU());*/
}

void GameScene::ImguiDraw() {
#ifdef _DEBUG

	ImGui::Begin("GameScene");
	ImGui::Text("Frame rate: %6.2f fps", ImGui::GetIO().Framerate);
	ImGui::Text("ColliderManagerSize : %d", (int)collision2DManager_->GetColliders().size());
	float abs = 256.0f;
	ImGui::DragFloat2("rShift", &rShift_.x, 0.01f, -abs, abs);
	ImGui::DragFloat2("gShift", &gShift_.x, 0.01f, -abs, abs);
	ImGui::DragFloat2("bShift", &bShift_.x, 0.01f, -abs, abs);
	ImGui::DragFloat2("shiftVelocity_", &shiftVelocity_.x, 0.01f, -abs, abs);
	ImGui::DragFloat("gti", &glitchTime_, 0.01f);
	ImGui::Checkbox("IsShift", &isShift_);
	ImGui::Checkbox("IsImpact", &isImpact_);
	ImGui::End();

	//Obj
	mapManager_->ImGuiDraw();
	// プレイヤー
	player_->ImGuiDraw();
	// 敵
	enemyManager_->ImGuiDraw();
	// ボス
	//bossEnemy_->ImGuiDraw();

	// 背景
	backGround_->ImGuiDraw();

	// スカイドーム
	//skydome_->ImGuiDraw();

	debugCamera_->ImGuiDraw();

	//collision2DDebugDraw_->ImGuiDraw();

	gameCamera_->ImGuiDraw();

	followCamera_->ImGuiDraw();

	gameData_->ApplyGlobalVariables();

	// ポストエフェクトのImGuiを表示
	PostEffect::GetInstance()->ImGuiDraw();
	// フォグのImGuiの表示
	FogManager::GetInstance()->ImGuiDraw();

#endif // _DEBUG

}

void GameScene::DebugCameraUpdate()
{

#ifdef _DEBUG
	if (input_->TriggerKey(DIK_SPACE)) {
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
		// 
		gameCamera_->Update();
		followCamera_->Update();
		// 
		camera_ = static_cast<BaseCamera>(*followCamera_.get());

		if (player_->GetEffectInfo().isStop && !camera_.IsShakeNow()) {
			camera_.ShakeStart(0.3f, 2);
		}
		player_->GetWeapon()->GetEffectSystem()->SetScreenPosition(camera_);

		// 
		camera_.Update();
	}

}

void GameScene::ModelCreate()
{

	// パーティクル
	particleUvcheckerModel_.reset(Model::Create("Resources/default/", "plane.gltf", dxCommon_, textureHandleManager_.get()));
	particleCircleModel_.reset(Model::Create("Resources/Particle/", "plane.obj", dxCommon_, textureHandleManager_.get()));

	// スカイドーム
	skydomeModel_.reset(Model::Create("Resources/Model/Skydome/", "skydome.obj", dxCommon_, textureHandleManager_.get()));

	// サンプルobj
	sampleObjModel_.reset(Model::Create("Resources/default/", "ball.gltf", dxCommon_, textureHandleManager_.get()));

	// プレイヤーモデル
	#ifndef _DEBUG // デバッグ以外の場合高負荷モデルを読み込む
	playerModel_.reset(Model::Create("Resources/Model/Player/", "Player.gltf", dxCommon_, textureHandleManager_.get()));
	ponyTailModel_.reset(Model::Create("Resources/Model/Player/", "PonyTail.gltf", dxCommon_, textureHandleManager_.get()));
	#endif // !_DEBUG
	#ifdef _DEBUG // デバッグの場合低負荷モデルを読み込む
	playerModel_.reset(Model::Create("Resources/default/", "ball.obj", dxCommon_, textureHandleManager_.get()));
	#endif // _DEBUG
	weaponModel_.reset(Model::Create("Resources/Model/Spear/", "Spear.gltf", dxCommon_, textureHandleManager_.get()));
	ringModel_.reset(Model::Create("Resources/Model/Spear/", "Ring.gltf", dxCommon_, textureHandleManager_.get()));
	#ifndef _DEBUG // デバッグ以外の場合高負荷モデルを読み込む

	#endif // !_DEBUG


	// 地形ブロック
	terrainModel_.reset(Model::Create("Resources/GameObject/Block", "Block.gltf", dxCommon_, textureHandleManager_.get()));

	// 背景モデル
	backGroundModel_.reset(Model::Create("Resources/Model/BackGround", "BackGround.obj", dxCommon_, textureHandleManager_.get()));

	// 敵モデル
	// プレイヤーモデル
	#ifndef _DEBUG // デバッグ以外の場合高負荷モデルを読み込む
	enemyModel_.reset(Model::Create("Resources/Model/Enemy/", "Enemy.gltf", dxCommon_, textureHandleManager_.get()));
	#endif // !_DEBUG
	#ifdef _DEBUG // デバッグの場合低負荷モデルを読み込む
	enemyModel_.reset(Model::Create("Resources/GameObject/cube", "cube.obj", dxCommon_, textureHandleManager_.get()));
	#endif // _DEBUG

}

void GameScene::TextureLoad()
{

	collision2DDebugDrawTextures_ = {
		TextureManager::Load("Resources/Debug/Box.png", DirectXCommon::GetInstance(), textureHandleManager_.get()),
		TextureManager::Load("Resources/Debug/Circle.png", DirectXCommon::GetInstance(), textureHandleManager_.get())
	};

	blockTexture_ = TextureManager::Load("Resources/default/white2x2.png", DirectXCommon::GetInstance(), textureHandleManager_.get());
	enemyTexture_ = TextureManager::Load("Resources/Model/Enemy/EnemyTex.png", DirectXCommon::GetInstance(), textureHandleManager_.get());

	//uiTextureHandles_ = {

	//};
}

void GameScene::LowerVolumeBGM()
{


	const uint32_t startHandleIndex = 3;

	//for (uint32_t i = 0; i < audioManager_->kMaxPlayingSoundData; ++i) {
	//	if (audioManager_->GetPlayingSoundDatas()[i].handle_ == kGameAudioNameIndexBGM + startHandleIndex) {
	//		float decreasingVolume = 1.0f / 60.0f;
	//		float volume = audioManager_->GetPlayingSoundDatas()[i].volume_ - decreasingVolume;
	//		if (volume < 0.0f) {
	//			volume = 0.0f;
	//			audioManager_->StopWave(i);
	//			isDecreasingVolume = false;
	//		}
	//		else {
	//			audioManager_->SetPlayingSoundDataVolume(i, volume);
	//			audioManager_->SetVolume(i, audioManager_->GetPlayingSoundDatas()[i].volume_);
	//		}
	//	}
	//}

}

void GameScene::ShadowUpdate()
{

	// リストクリア
	//shadowManager_->ListClear();

	// リスト登録（影を発生させる物）
	//shadowManager_->CastsShadowObjListRegister();

	// リスト登録（影が現れる物）
	//shadowManager_->ShadowAppearsObjListRegister();

	// 影が出るか
	//shadowManager_->SeeShadow();

}

void GameScene::CollisionUpdate()
{

	// あたり判定
	collisionManager_->ListClear();
	collisionManager_->CheakAllCollision();

	collision2DManager_->ListClear();
	// プレイヤー
	collision2DManager_->ListRegister(&player_->cameraRay_.directRay_);
	collision2DManager_->ListRegister(&player_->circleCollider_);
	// 武器
	if (!std::holds_alternative<HoldState*>(player_->GetWeapon()->GetNowState())) {
		collision2DManager_->ListRegister(&player_->GetWeapon()->boxCollider_);
	}
	// プレイヤーの足場
	collision2DManager_->ListRegister(&player_->GetFootCollider()->boxCollider_);

	// マップ
	mapManager_->CollisionRegister(collision2DManager_.get(), camera_);

	enemyManager_->CollisionRegister(collision2DManager_.get(), camera_);

	collision2DManager_->CheakAllCollision();

	collision2DDebugDraw_->Clear();

}
