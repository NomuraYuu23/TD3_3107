#pragma once
#include "../../../Engine/Input/Input.h"
#include "../../../Engine/3D/Model.h"

class Player;

class PlayerController
{
public:
	PlayerController() {};
	~PlayerController() = default;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="player"></param>
	void Initialize(Player* player);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	void ImGuiDraw();

public:
	struct ActionButton {
		bool jumpButton;
		bool throwButton;
	};

	struct SettingButton {
		int32_t jumpKey;	// 飛び
		int32_t throwKey;	// 投げ
	};

	enum ThrowType
	{
		kDefault,
		kButtonRelease,
		kStickRelease,
	};

	int32_t throwType_ = ThrowType::kButtonRelease;

private:
	/// <summary>
	/// 戻しの処理
	/// </summary>
	void GetBackWeaponProcess();

	/// <summary>
	/// ボタンの設定（毎フレーム
	/// </summary>
	void KeySetting();

	/// <summary>
	/// キーの設定
	/// </summary>
	void KeyConfigSetting();

private: // 外付け機器の処理（大枠
	/// <summary>
	/// コントローラー操作処理
	/// </summary>
	void ControllerProcess();

private: // ステートごとの処理
	/// <summary>
	/// 空中での操作
	/// </summary>
	void AerialMoveProcess();
	/// <summary>
	/// 地上での操作
	/// </summary>
	void GroundMoveProcess();

	/// <summary>
	/// 槍に乗った待機状態の操作
	/// </summary>
	void WaitKeyProcess();


private: // 操作内容関数
	/// <summary>
	/// 投げの処理
	/// </summary>
	void ThrownProcess();

private:
	// インプットクラス
	Input* input_ = nullptr;
	// プレイヤーのポインタ
	Player* player_ = nullptr;
private:
	// 地上の速度
	float groundSpeed_ = 0;
	// 空中の速度
	float aerialSpeed_ = 0;
	// 入力
	ActionButton actionButton_;
	// キーの内容
	SettingButton setButton_;

	// 前フレームの右スティック
	Vector2 prevRightStick_ = {};

	// スティックで戻す処理のONかOFFフラグ
	bool isStickBack_ = false;

public:

	// 短縮処理
	bool isShortening_ = false;

};

