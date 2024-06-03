#include "GameScene.h"
#include "../../../Engine/base/WinApp.h"
#include "../../../Engine/base/TextureManager.h"
#include "../../../Engine/2D/ImguiManager.h"
#include "../../../Engine/base/D3DResourceLeakChecker.h"
#include "../../../Engine/GlobalVariables/GlobalVariables.h"
#include "../../Particle/EmitterName.h"
#include "../../../Engine/Math/DeltaTime.h"
#include "../../../Engine/base/WindowSprite.h"
#include "../../AllSceneObject/StageNumberManager.h"

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
	particleManager_->Finalize();
	std::array<Model*, ParticleModelIndex::kCountofParticleModelIndex> particleModel;
	particleModel[ParticleModelIndex::kUvChecker] = particleUvcheckerModel_.get();
	particleModel[ParticleModelIndex::kCircle] = particleCircleModel_.get();
	particleModel[ParticleModelIndex::kBambooLeaf] = particleLeafModel_.get();
	particleModel[ParticleModelIndex::kSpearLeaf] = particleSpearLeafModel_.get();
	particleModel[ParticleModelIndex::kSmoke] = particleSmokeModel_.get();
	particleModel[ParticleModelIndex::kKiraKira] = particleKiraKiraModel_.get();
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
	#ifdef _RELEASE
	// リングモデルを渡す
	weapon->SetRingModel(ringTopModel_.get(), ringUnderModel_.get());
	#endif // !_DEBUG


	// 生成
	player_ = std::make_unique<Player>();
	// 武器の設定
	player_->SetWeapon(std::move(weapon));
	// 初期化
	player_->Initialize(playerModel_.get());
	// デバッグ以外の場合行う
	#ifdef _RELEASE
	// ポニーテール
	player_->SetPonyTail(ponyTailModel_.get());
	// オーディオマネージャーを渡す
	player_->gameAudioManager_ = audioManager_.get();
	#endif // !_DEBUG
	// 矢印モデルをセット
	player_->SetArrowModel(arrowModel_.get());

	player_->GetSlowEffect()->SetCamera(&camera_);


	// 更新
	//countTime_ = 0;

	// ゲームシステム
	gameSystemManager_ = std::make_unique<GameSystemManager>();
	gameSystemManager_->Initialize(goalModel_.get(), checkPointModel_.get(), player_.get(), audioManager_.get());

	// 敵管理クラス
	enemyTextures_[0].clear();
	enemyTextures_[0].push_back(singleEnemyTexture_);
	enemyTextures_[2].clear();
	enemyTextures_[2].push_back(chaseTexture_);
	enemyManager_ = std::make_unique<EnemyManager>();
	enemyManager_->SetPlayer(player_.get());
	enemyManager_->Initialize(enemyModel_.get(), &enemyTextures_[0], &enemyTextures_[2]);

	player_->SetEnemyManager(enemyManager_.get());
	player_->Update();

	//bossEnemy_ = std::make_unique<PrevSmallBoss>();
	//bossEnemy_->Initialize(enemyModel_.get());

	// マップ管理クラス
	mapManager_ = std::make_unique<MapManager>();
	mapManager_->blockTexture_ = TextureManager::Load("Resources/default/white2x2.png", DirectXCommon::GetInstance(), textureHandleManager_.get());
	mapManager_->Initialize(terrainModel_.get());

	// 背景用オブジェクト
	backGround_ = std::make_unique<BackGround>();
	backGround_->Initialize(backGroundModel_.get());
	if (StageNumberManager::stageNum_ == 1) {
		// 最初のステージならチュートリアルモデルを設定
		backGround_->SetTutorialPlaneModel(textureHandleManager_.get(), spearJumpTutorialPlaneModel_.get(), enemyTutorialPlaneModel_.get());
	}
	// 定点カメラ（仮
	gameCamera_ = std::make_unique<GameBasicCamera>();
	gameCamera_->Initialize();

	// 追従カメラ（仮
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();
	followCamera_->SetPlayer(player_.get());

	/// ポストエフェクトの値初期化
	// ブルーム
	PostEffect* pe = PostEffect::GetInstance();
	pe->SetThreshold(0.05f);
	pe->SetKernelSize(5);
	pe->SetSigma(5.0f);

	FogManager* fm = FogManager::GetInstance();
	fm->SetColor({ 0.0f, 0.25f, .65f, 1.0f });
	fm->SetNear(50.0f);
	fm->SetFar(2500.0f);

	// ゲームシーン用BGMの再生
	audioManager_->PlayWave(kGameSceneBGM);

	// Jsonデータのクラス
#ifdef _DEMO

	gameData_ = GameObjectData::GetInstance();
	gameData_->Initialize();

#endif // _DEMO
	skydome_->SetParent(&player_->worldtransform_);

	// ゲームシステムにポインタ登録
	gameSystemManager_->SetEnemyManager(enemyManager_.get());

	// UIマネージャーの生成
	gameUIManager_ = std::make_unique<GameUIManager>();		// 生成
	gameUIManager_->SetPlayer(player_.get());				// プレイヤーセット
	gameUIManager_->Initialze(textureHandleManager_.get()); // 初期化
	player_->SetUIManager(gameUIManager_.get());			// UIマネージャーセット

	// ゲームシステムにUIマネージャーをセット
	gameSystemManager_->SetGameUIManager(gameUIManager_.get());
	// ゲームシステムにカメラをセット
	gameSystemManager_->SetFollowCamera(followCamera_.get());
}

/// <summary>
/// 更新処理
/// </summary>
void GameScene::Update() {

#ifdef _DEMO
	ImguiDraw();

	if (input_->TriggerKey(DIK_L)) {
		requestSceneNo_ = kTitle;
	}


#endif

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
	if (input_->TriggerKey(DIK_R)) {
		resetScene_ = true;
		isBeingReset_ = true;
		isDecreasingVolume = true;
	}

	if (gameSystemManager_->GetIsGameClear()) {
		requestSceneNo_ = kSelect;
	}

	directionalLight_->Update(directionalLightData_);

	pointLightManager_->Update(pointLightDatas_);
	spotLightManager_->Update(spotLightDatas_);
	// UIマネージャー更新
	gameUIManager_->Update();

	// ゲームシステム
	gameSystemManager_->Update();

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
	if (!player_->GetIsDead() && !gameUIManager_->GetDisplayPoseUI()) { // 死亡していないときのみ更新
		player_->Update();
	}
	player_->DrawLinesMap(drawLine_);
	// 敵
	if (!player_->isGameClear_ && !player_->GetIsDead() && !gameUIManager_->GetDisplayPoseUI()) {
		enemyManager_->Update();
	}


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
	preDrawDesc.environmentTextureHandle = skyboxTextureHandle_;

	ModelDraw::PreDraw(preDrawDesc);

	//3Dオブジェクトはここ

	// スカイドーム
	skydome_->Draw(camera_);

	// 背景
	backGround_->Draw(camera_);

	//bossEnemy_->Draw(camera_);

	// ブロック用
	mapManager_->Draw(camera_);
	// ゴール系
	gameSystemManager_->Draw(camera_);

	enemyTextures_[1].clear();
	enemyTextures_[1].push_back(enemyTexture_);

	// 敵
	enemyManager_->Draw(camera_, &enemyTextures_[1]);

	// プレイヤーのリングは透過するため最後に描画
	player_->weapon_->UnderRingDraw(camera_);

	//Obj
	player_->Draw(camera_);

	// プレイヤーのリングは透過するため最後に描画
	player_->weapon_->TopRingDraw(camera_);

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

	// UIマネージャー描画
	gameUIManager_->Draw();
	//arrowSprite_->Draw();

	// 前景スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion

	uint32_t postEffectBit = 0;
	PostEffect::ExecutionAdditionalDesc desc = {};
	desc.shockWaveManagers[0] = player_->GetWeapon()->GetEffectSystem()->GetShockWaveManager();
	desc.shockWaveManagers[1] = player_->GetWeaponGuardEffect()->GetShockWaveManager();
	desc.velocity2DManagers[0] = player_->GetVelocity2DManager();

	if (player_->GetHitManager().IsHitEffectActive()) {
		PlayerHitManager::Effect instance = player_->GetEffectInfo();
		PostEffect::GetInstance()->SetRShift(instance.rShift);
		PostEffect::GetInstance()->SetGShift(instance.gShift);
		PostEffect::GetInstance()->SetBShift(instance.rShift);
		PostEffect::GetInstance()->SetTime(instance.time);
		postEffectBit += 6;
	}
	if (player_->GetWeapon()->GetEffectSystem()->IsActive()) {
		postEffectBit += 1;
	}
	if (player_->GetSlowEffect()->GetRunning()) {
		PostEffect::GetInstance()->SetColorPosition(player_->GetSlowEffect()->GetCenter());
		PostEffect::GetInstance()->SetColorSize(player_->GetSlowEffect()->GetSize());
		PostEffect::GetInstance()->SetColorLerpT(player_->GetSlowEffect()->GetColorT());
		postEffectBit += 8;
	}

	if (std::holds_alternative<SpearAerialState*>(player_->GetNowState()) && !player_->IsDead()) {
		postEffectBit += 16;
		PostEffect::GetInstance()->SetKernelSize(33);
		PostEffect::GetInstance()->SetSigma(33.0f);
	}

	if (player_->GetWeaponGuardEffect()->IsActive()) {
		postEffectBit += 32;
	}

	PostEffect::GetInstance()->SetExecutionFlag(postEffectBit);
	PostEffect::GetInstance()->Execution(
		dxCommon_->GetCommadList(),
		renderTargetTexture_,
		PostEffect::kCommandIndexTAKEYARIMONOGATARI_First,
		&desc);

	WindowSprite::GetInstance()->DrawUAV(PostEffect::GetInstance()->GetEditTextures(0)->GetUavHandleGPU());

}

void GameScene::ImguiDraw() {

#ifdef _DEMO

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

	gameSystemManager_->ImGuiDraw();

	//collision2DDebugDraw_->ImGuiDraw();

	//gameCamera_->ImGuiDraw();

	followCamera_->ImGuiDraw();

	gameData_->ApplyGlobalVariables();

	StageNumberManager::ImGuiDraw();

	// ポストエフェクトのImGuiを表示
	PostEffect::GetInstance()->ImGuiDraw();
	// フォグのImGuiの表示
	FogManager::GetInstance()->ImGuiDraw();

	// UIマネージャー用
	gameUIManager_->DisplayImGui();

#endif // _DEBUG

}

void GameScene::DebugCameraUpdate()
{

#ifdef _DEMO
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
		player_->GetWeapon()->GetEffectSystem()->SetCameraAddPosition(followCamera_->GetDefaultOffsetAdd());

		player_->GetWeaponGuardEffect()->SetScreenPosition(camera_);
		player_->GetWeaponGuardEffect()->SetCameraAddPosition(followCamera_->GetDefaultOffsetAdd());


		// 
		//camera_.Update();
	}

}

void GameScene::ModelCreate()
{

	// パーティクル
	particleUvcheckerModel_.reset(Model::Create("Resources/default/", "plane.gltf", dxCommon_, textureHandleManager_.get()));
	particleCircleModel_.reset(Model::Create("Resources/Particle/", "plane.obj", dxCommon_, textureHandleManager_.get()));
	particleLeafModel_.reset(Model::Create("Resources/Particle/BambooLeaf", "BambooLeaf.obj", dxCommon_, textureHandleManager_.get()));
	particleSpearLeafModel_.reset(Model::Create("Resources/Particle/SpearLeaf", "SpearLeaf.obj", dxCommon_, textureHandleManager_.get()));
	particleSmokeModel_.reset(Model::Create("Resources/Particle/Smoke", "Smoke.obj", dxCommon_, textureHandleManager_.get()));
	particleKiraKiraModel_.reset(Model::Create("Resources/Particle/KiraKira", "KiraKira.obj", dxCommon_, textureHandleManager_.get()));

	// スカイドーム
	skydomeModel_.reset(Model::Create("Resources/Model/Skydome/", "skydome.obj", dxCommon_, textureHandleManager_.get()));

	// サンプルobj
	sampleObjModel_.reset(Model::Create("Resources/default/", "ball.gltf", dxCommon_, textureHandleManager_.get()));

	// プレイヤーモデル
	#ifdef _RELEASE // デバッグ以外の場合高負荷モデルを読み込む
	playerModel_.reset(Model::Create("Resources/Model/Player/", "Player.gltf", dxCommon_, textureHandleManager_.get()));
	ponyTailModel_.reset(Model::Create("Resources/Model/Player/", "PonyTail.gltf", dxCommon_, textureHandleManager_.get()));
	#endif // !_DEBUG
	#ifndef _RELEASE // デバッグの場合低負荷モデルを読み込む
	playerModel_.reset(Model::Create("Resources/default/", "ball.obj", dxCommon_, textureHandleManager_.get()));
	#endif // _DEBUG
	weaponModel_.reset(Model::Create("Resources/Model/Spear/", "Spear.gltf", dxCommon_, textureHandleManager_.get()));
	ringUnderModel_.reset(Model::Create("Resources/Model/SpearRing/", "SpearRingUnder.obj", dxCommon_, textureHandleManager_.get()));
	ringTopModel_.reset(Model::Create("Resources/Model/SpearRing/", "SpearRingTop.obj", dxCommon_, textureHandleManager_.get()));
	arrowModel_.reset(Model::Create("Resources/Model/Direction/", "Direction.obj", dxCommon_, textureHandleManager_.get()));


	// 地形ブロック
	terrainModel_.reset(Model::Create("Resources/GameObject/Block", "Block.gltf", dxCommon_, textureHandleManager_.get()));

	// 背景モデル
	backGroundModel_.reset(Model::Create("Resources/Model/BackGround", "BackGround.obj", dxCommon_, textureHandleManager_.get()));

	checkPointModel_.reset(Model::Create("Resources/Model/Takenoko", "Takenoko.obj", dxCommon_, textureHandleManager_.get()));
	goalModel_.reset(Model::Create("Resources/Model/Goal", "Goal.gltf", dxCommon_, textureHandleManager_.get()));

	// ステージ番号が 00 のときのみロード
	if (StageNumberManager::stageNum_ == 1) {
		spearJumpTutorialPlaneModel_.reset(Model::Create("Resources/Model/TutorialPlane", "TutorialPlane.obj", dxCommon_, textureHandleManager_.get()));
		enemyTutorialPlaneModel_.reset(Model::Create("Resources/Model/TutorialPlane", "TutorialPlane.obj", dxCommon_, textureHandleManager_.get()));
	}

	// 敵モデル
	// プレイヤーモデル
	#ifdef _RELEASE // デバッグ以外の場合高負荷モデルを読み込む
	enemyModel_.reset(Model::Create("Resources/Model/Enemy/", "Enemy.gltf", dxCommon_, textureHandleManager_.get()));
	#endif // !_DEBUG
	#ifndef _RELEASE // デバッグの場合低負荷モデルを読み込む
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
	enemyTexture_ = TextureManager::Load("Resources/Model/Enemy/EnemyRedTex.png", DirectXCommon::GetInstance(), textureHandleManager_.get());

	skyboxTextureHandle_ = TextureManager::Load("Resources/default/rostock_laage_airport_4k.dds", DirectXCommon::GetInstance(), textureHandleManager_.get());

	singleEnemyTexture_ = TextureManager::Load("Resources/Model/Enemy/EnemyBlueTex.png", DirectXCommon::GetInstance(), textureHandleManager_.get());

	chaseTexture_ = TextureManager::Load("Resources/Model/Enemy/EnemyTex.png", DirectXCommon::GetInstance(), textureHandleManager_.get());
	//uiTextureHandles_ = {

	//};
}

void GameScene::LowerVolumeBGM()
{

	const uint32_t startHandleIndex = 6;

	uint32_t index = kGameSceneBGM + startHandleIndex;

	for (uint32_t i = 0; i < audioManager_->kMaxPlayingSoundData; ++i) {
		if (audioManager_->GetPlayingSoundDatas()[i].handle_ == index) {
			float decreasingVolume = 1.0f / 60.0f;
			float volume = audioManager_->GetPlayingSoundDatas()[i].volume_ - decreasingVolume;
			if (volume < 0.0f) {
				volume = 0.0f;
				audioManager_->StopWave(i);
				isDecreasingVolume = false;
			}
			else {
				audioManager_->SetPlayingSoundDataVolume(i, volume);
				audioManager_->SetVolume(i, audioManager_->GetPlayingSoundDatas()[i].volume_);
			}
		}
	}

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

	// ゲームシステム関係（ゴール・チェックポイント）
	gameSystemManager_->CollisionRegister(collision2DManager_.get());

	collision2DManager_->CheakAllCollision();

	collision2DManager_->ListClear();

}
