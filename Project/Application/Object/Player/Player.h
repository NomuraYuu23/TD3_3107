#pragma once
#include "../IObject.h"
#include "../Weapon/Weapon.h"
#include "../GameUtility/TimerLib.h"
#include "../../../Engine/Collider2D/Segment2D.h"
#include "../../../Engine/Input/Input.h"
#include "../../../Engine/GlobalVariables/GlobalVariables.h"

#include "ActionState/ActionStateList.h"
#include "System/PlayerSystemList.h"
#include "../GameSystem/GameSystemManager.h"

#include "PlayerFootCollider.h"

#include "Anim/PlayerAnimManager.h"

#include "../../../Engine/Physics/String.h"

#include "../../AudioManager/GameAudioManager.h"

#include "System/SlowEffect/SlowEffect.h"

class EnemyManager;
class GameUIManager;

class Player : public IObject
{
private: // サブクラス

public: // 継承
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	void Initialize(Model* model) override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	void Draw(const BaseCamera& camera) override;
	/// <summary>
	/// ImGui
	/// </summary>
	void ImGuiDraw() override;
	/// <summary>
	/// 衝突判定
	/// </summary>
	/// <param name="target"></param>
	/// <param name="tag"></param>
	void OnCollision(ColliderParentObject2D target) override;

public: // アクセッサ
	Vector2 GetColliderPosition() override { return circleCollider_.position_; }
	Vector2 GetColliderSize() override { return boxCollider_.scale_; }
	Box GetBoxCollider() override { return boxCollider_; }

	PlayerHitManager::Effect GetEffectInfo() { return hpManager_.hitEffect_; }
	PlayerHitManager GetHitManager() { return hpManager_; }

	SlowEffect* GetSlowEffect() { return slowEffect_.get(); }

	/// <summary>
	/// 現在体力ゲッター
	/// </summary>
	/// <returns>プレイヤーの現在体力</returns>
	int32_t GetCurrentHealth() { return hpManager_.GetCurrentHealth(); }

	/// <summary>
	/// 現在体力ゲッター
	/// </summary>
	/// <returns>プレイヤーの現在体力</returns>
	int32_t GetMaxHealth() { return hpManager_.GetMaxHealth(); }

public: // メンバ関数
	/// <summary>
	/// ステートの変更
	/// </summary>
	/// <param name="newState"></param>
	void ChangeState(std::unique_ptr<IActionState> newState);

	/// <summary>
	/// 武器の作成（外部で行う想定）
	/// </summary>
	/// <param name="newWeapon"></param>
	void SetWeapon(std::unique_ptr<Weapon> newWeapon) {
		weapon_ = std::move(newWeapon);
		weapon_->SetParentAdress(&worldtransform_);
		weapon_->SetPlayer(this);
	}

	/// <summary>
	/// 武器のポインタ取得
	/// </summary>
	Weapon* GetWeapon() { return weapon_.get(); }

	/// <summary>
	/// 現在の状態
	/// </summary>
	/// <returns></returns>
	PlayerState GetNowState() { return nowState_; }

	/// <summary>
	/// 足元のコライダー
	/// </summary>
	/// <returns></returns>
	PlayerFootCollider* GetFootCollider() { return &footCollider_; }

	/// <summary>
	/// ステートの設定
	/// </summary>
	/// <param name="state"></param>
	void SetNowState(PlayerState state) { nowState_ = state; }

	/// <summary>
	/// 反動中か
	/// </summary>
	bool IsRecoil() { return recoil_.IsActive(); }

	/// <summary>
	/// 線描画
	/// </summary>
	/// <param name="drawLine">線描画クラス</param>
	void DrawLinesMap(DrawLine* drawLine);

	/// <summary>
	/// 死んだ時に呼び出すやつ（リスポーン）
	/// </summary>
	/// <param name="position">位置</param>
	void Reset(const Vector3& position);

public: // アニメーション関連関数群

	/// <summary>
	/// アニメーションマネージャーゲッター
	/// </summary>
	/// <returns>アニメーションマネージャー</returns>
	PlayerAnimManager* GetAnimManager() { return anim_.get(); }

	/// <summary>
	/// 補正システム
	/// </summary>
	/// <returns></returns>
	SpearLandingAdjuster GetLandingAdjuster() { return landingAdjuster_; }

public:
	// 矢印モデル
	//void SetArrowModel(Model* arrow) { arrow_.plane_ = arrow; }
	// スクリーン座標
	Vector2 screenPos_ = {};

	// コンボ用の呼び出し関数
	void AddCombo() { jumpCombo_.Add(); }
	void ResetCombo() { jumpCombo_.Reset(); }

	void SetEnemyManager(EnemyManager* enemyManager) { enemyManager_ = enemyManager; }

	bool IsFreeFallTimerEnd() { return fallTimer_.IsEnd(); }
	bool FreeFallActive() { return fallTimer_.IsActive(); }

	void SetFallTimer();

	/// <summary>
	/// 矢印UIの更新関数
	/// </summary>
	void ArrowUIUpdate();

	/// <summary>
	/// 死亡フラグの設定
	/// </summary>
	/// <param name="isDead"></param>
	void SetIsDead(bool isDead) { isDead_ = isDead; }

	bool IsCanReturn() { return (!knockBackSystem_.IsHit() && !recoil_.IsActive()); }

	void KnockBackOnGround() { knockBackSystem_.SetIsHit(false); }

	/// <summary>
	/// ポニーテール用のモデルセッター
	/// </summary>
	/// <param name="model">モデル</param>
	void SetPonyTail(Model* model);

	/// <summary>
	/// UIマネージャーセッター
	/// </summary>
	/// <param name="uiManager">UIマネージャー</param>
	void SetUIManager(GameUIManager* uiManager) { uiManager_ = uiManager; }
	/// <summary>
	/// UIマネージャーゲッター
	/// </summary>
	/// <returns>UIマネージャー</returns>
	GameUIManager* GetUIManager() { return uiManager_; }

	bool IsNowAssistDash() { return assistDash_.IsFallslowActive(); }
	void EndAssistDash() { assistDash_.SlowCancel(); }

	Velocity2DManager* GetVelocity2DManager() { return velocity2DManager_.get(); }

	/// <summary>
	/// 矢印モデルセッター
	/// </summary>
	/// <param name="model">矢印モデル</param>
	void SetArrowModel(Model* model);

public:
	// ステート
	std::unique_ptr<IActionState> actionState_;
	// 武器
	std::unique_ptr<Weapon> weapon_;
	// 投げる方向
	Vector3 throwDirect_ = { 1,0,0 };
	// 前座標
	Vector3 prevPosition_ = {};

	bool isLeft_ = false;
	// 接地フラグ
	bool isGround_ = false;
	// 矢印描画フラグ
	bool isArrowUiDraw_ = false;
	// 一度踏んだかのフラグ
	bool isOneStepOn_ = false;

	// 足元コライダー
	PlayerFootCollider footCollider_;

	float floorPrevY_ = 0;

	float rayLength_ = 0;

	CameraRay cameraRay_;

	EnemyManager* enemyManager_;

	TimerLib spearJumpAccepter_;

	// オーディオマネージャー
	GameAudioManager* gameAudioManager_ = nullptr;

private: // フラグ
	// ゲームスピード
	bool isSlowGame_ = false;
	// デバッグ用
	bool isDebugDraw_ = false;
public:
	bool isSlowNow_ = false;

private: // システム
	// 現状のステート
	PlayerState nowState_;
	// 操作クラス
	PlayerController controller_;
	// 放物線
	PlayerParabola parabola_;
	// ジャンプ回数カウント
	ComboCounter jumpCombo_;
	// 自由落下の武器を回収するためのシステム
	FreeFallTimer fallTimer_;
	// スローエフェクト
	std::unique_ptr<SlowEffect> slowEffect_;

	//--- オンヒットシステム ---//
	// 反動管理クラス
	PlayerRecoil recoil_;
	// HP管理クラス
	PlayerHitManager hpManager_;
	// 敵とぶつかった時のノックバック
	KnockBack knockBackSystem_;

	// プレイヤーと槍をつなぐ線の色
	Vector4 connectingSpearLineColor_;

	// 補正用システム
	CorrectSystem correctSystem_;

	// 槍着地補正システム
	SpearLandingAdjuster landingAdjuster_;

private: // UI関連

	// UIマネージャー
	GameUIManager* uiManager_ = nullptr;

	// 投げる方向の矢印UI
	Model* arrowModel_;
	// 投げる方向の矢印トランスフォーム
	WorldTransform arrowTransform_;
	// 投げる方向の矢印マテリアル
	std::unique_ptr<Material> arrowMaterial_;
	// 矢印用UVトランスフォーム
	WorldTransform arrowUVTransform_;
	// 線の方向ベクトル
	Vector3 subArrowVector_;

private: // アニメーション関連

	// ポニーテール用紐クラス
	std::unique_ptr<String> ponytail_;
	// ポニテ用座標
	Vector3 ponyAnchorPos_ = {};

	// アニメーションマネージャー
	std::unique_ptr<PlayerAnimManager> anim_;

private: // パーティクル生成関連変数

	// 環境パーティクル生成用座標
	std::unique_ptr<EulerTransform> emitTransform_;
	// 環境パーティクル用設定構造体
	EmitterDesc fallingLeafDesc_;

	// 空中ダッシュシステム
	AssistDash assistDash_;

private: // モーションブラー

	std::unique_ptr<Velocity2DManager> velocity2DManager_;

private:
	void SystemInitialize();
	void SystemUpdate();
public:
	void HitUpdate() {
		hpManager_.Update();
	}
};

