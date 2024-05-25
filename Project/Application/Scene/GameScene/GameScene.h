#pragma once

#include "../../../Engine/Scene/IScene/IScene.h"
#include "../../../Engine/Collider/ColliderDebugDraw/ColliderDebugDraw.h"// コライダーデバッグ描画
#include"../../../Engine/Collision/CollisionManager.h"
#include "../../../Engine/base/ITextureHandleManager.h"
#include "../../../Engine/Collision/CollisionManager.h"
#include "../../UI/UIManager.h"
#include "../../AudioManager/GameAudioManager.h"
#include "../../Skydome/Skydome.h"

//#include "../../Object/Sample/SampleObject.h" // サンプルオブジェクト
#include "../../Object/ObjectList.h"	// オブジェクトフォルダ内のインクルード
#include "../../Object/GameObjectData.h"
#include "../../Object/GameCamera/GameBasicCamera.h"
#include "../../Object/GameSystem/GameSystemManager.h"

#include "../../Object/Enemy/EnemyManager.h"

#include "../../../Engine/Light/DirectionalLight/DirectionalLight.h" // 平行光源
#include "../../../Engine/Light/PointLight/PointLightManager.h" // 点光源
#include "../../../Engine/Light/SpotLight/SpotLightManager.h" // スポット

#include "../../../Engine/Collision2D/Collision2DManager.h"
#include "../../../Engine/Collision2D/Collision2DDebugDraw.h"

#include "../../UI/GameUIManager.h"

class GameScene : public IScene
{

public:

	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// imgui描画処理
	/// </summary>
	void ImguiDraw();

public: //メンバ関数

	/// <summary>
	/// デバッグカメラ更新
	/// </summary>
	void DebugCameraUpdate();

private: // メンバ関数

	/// <summary>
	/// モデルクリエイト
	/// </summary>
	void ModelCreate() override;

	/// <summary>
	/// テクスチャロード
	/// </summary>
	void TextureLoad() override;

private:
	  
	/// <summary>
	/// 音量
	/// </summary>
	void LowerVolumeBGM();

	/// <summary>
	/// 影更新
	/// </summary>
	void ShadowUpdate();

private:

	void CollisionUpdate();

private:
	// json保存用のクラス
	GameObjectData* gameData_ = nullptr;
	// パーティクルマネージャー
	ParticleManager* particleManager_ = nullptr;
	std::unique_ptr<Model> particleUvcheckerModel_ = nullptr;
	std::unique_ptr<Model> particleCircleModel_ = nullptr;
	std::unique_ptr<Model> particleLeafModel_ = nullptr;

	std::unique_ptr<CollisionManager> collisionManager_;

	// UIマネージャー
	//std::unique_ptr<UIManager> uiManager_;
	//std:::array<uint32_t, UITextureHandleIndex::kUITextureHandleIndexOfCount> uiTextureHandles_;

	// オーディオマネージャー
	std::unique_ptr<GameAudioManager> audioManager_;
	bool isDecreasingVolume = true;

	// スカイドーム
	std::unique_ptr<Skydome> skydome_;
	std::unique_ptr<Model> skydomeModel_;

	// サンプルOBJ
	std::unique_ptr<SampleObject> sampleObj_;
	std::unique_ptr<Model> sampleObjModel_;

	// Collision2DManager
	std::unique_ptr<Collision2DManager> collision2DManager_;
	// Collision2DDebugDraw
	std::unique_ptr<Collision2DDebugDraw> collision2DDebugDraw_;
	std::array<uint32_t, Collision2DDebugDraw::kTexutureNameOfCount> collision2DDebugDrawTextures_;
	
	// ゲーム管理
	std::unique_ptr<GameSystemManager> gameSystemManager_;

	// プレイヤー
	std::unique_ptr<Player> player_;
	std::unique_ptr<Model> playerModel_;
	std::unique_ptr<Model> ponyTailModel_;
	std::unique_ptr<Model> weaponModel_;
	std::unique_ptr<Model> ringUnderModel_;
	std::unique_ptr<Model> ringTopModel_;

	// 地形・ブロック
	std::unique_ptr<MapManager> mapManager_;
	std::unique_ptr<Model> terrainModel_;

	// 背景
	std::unique_ptr<BackGround> backGround_;
	std::unique_ptr<Model> backGroundModel_;
	std::unique_ptr<Model> goalModel_;

	// 敵
	std::unique_ptr<EnemyManager> enemyManager_;
	std::unique_ptr<Model> enemyModel_;
	std::unique_ptr<IBoss> bossEnemy_;

	// カメラ
	std::unique_ptr<GameBasicCamera> gameCamera_;
	std::unique_ptr<FollowCamera> followCamera_;

	// UIマネージャー
	std::unique_ptr<GameUIManager> gameUIManager_;

	std::unique_ptr<Sprite> arrowSprite_;
	uint32_t arrowTexture_ = 0u;

	float countTime_ = 0.0f;

	uint32_t blockTexture_ = 0u;
	uint32_t enemyTexture_ = 0u;

	std::vector<UINT> tmpTextures_;

	Vector2 rShift_ = {};
	Vector2 gShift_ = {};
	Vector2 bShift_ = {};

	Vector2 shiftVelocity_ = {};
	bool isShift_ = false;
	float glitchTime_ = 0.0f;

	bool isImpact_ = false;

};
