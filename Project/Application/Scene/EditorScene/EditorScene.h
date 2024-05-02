#pragma once
#include "../../../Engine/Scene/IScene/IScene.h"
#include "../../../Engine/Collider/ColliderDebugDraw/ColliderDebugDraw.h"// コライダーデバッグ描画
#include"../../../Engine/Collision/CollisionManager.h"
#include "../../../Engine/base/ITextureHandleManager.h"
#include "../../../Engine/Collision/CollisionManager.h"
#include "../../UI/UIManager.h"
#include "../../AudioManager/GameAudioManager.h"
#include "../../Skydome/Skydome.h"
#include "../../../Engine/3D/OutLineData.h"
#include "../../ShadowManager/ShadowManager.h"

#include "../../../Engine/Light/DirectionalLight/DirectionalLight.h" // 平行光源
#include "../../../Engine/Light/PointLight/PointLightManager.h" // 点光源
#include "../../../Engine/Light/SpotLight/SpotLightManager.h" // スポット

class EditorScene : public IScene
{
public:

	~EditorScene();

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
	// ライト
	std::unique_ptr<DirectionalLight> directionalLight_;
	Vector3 direction = { 1.0f, -1.0f, 0.0f };
	float intencity = 1.0f;
	// スカイドーム
	std::unique_ptr<Skydome> skydome_;
	std::unique_ptr<Model> skydomeModel_;

	// サンプルOBJ
	//std::unique_ptr<SampleObject> sampleObj_;
	std::unique_ptr<Model> sampleObjModel_;

	// 点光源
	std::unique_ptr<PointLightManager> pointLightManager_;
	std::array<PointLightData, PointLightManager::kNumInstanceMax_> pointLightDatas_;

	// スポットライト
	std::unique_ptr<SpotLightManager> spotLightManager_;
	std::array<SpotLightData, SpotLightManager::kNumInstanceMax_> spotLightDatas_;

};