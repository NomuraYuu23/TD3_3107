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

class Player : public IObject
{
private: // サブクラス
	// UI
	struct ArrowUIData {
		Model* plane_;
		Vector3 v3Position;
		float offsetLength;
	};

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

public: // アニメーション関連関数群

	/// <summary>
	/// アニメーションマネージャーゲッター
	/// </summary>
	/// <returns>アニメーションマネージャー</returns>
	PlayerAnimManager* GetAnimManager() { return anim_.get(); }

public:
	// 矢印モデル
	void SetArrowModel(Model* arrow) { arrow_.plane_ = arrow; }
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


	bool IsNowAssistDash() { return assistDash_.IsFallslowActive(); }
	void EndAssistDash() { assistDash_.SlowCancel(); }

public:
	// ステート
	std::unique_ptr<IActionState> actionState_;
	// 武器
	std::unique_ptr<Weapon> weapon_;
	// 投げる方向
	Vector3 throwDirect_ = { 1,0,0 };
	// 前座標
	Vector3 prevPosition_ = {};
	// 矢印テクスチャ
	uint32_t arrowTexture_ = 0u;

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
	// 矢印UI
	ArrowUIData arrow_;
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

private: // アニメーション関連

	// ポニーテール用紐クラス
	std::unique_ptr<String> ponytail_;
	// ポニテ用座標
	Vector3 ponyAnchorPos_ = {};

	// アニメーションマネージャー
	std::unique_ptr<PlayerAnimManager> anim_;

	// 空中ダッシュシステム
	AssistDash assistDash_;

private:
	void SystemInitialize();
	void SystemUpdate();
public:
	void HitUpdate() {
		hpManager_.Update();
	}
};

