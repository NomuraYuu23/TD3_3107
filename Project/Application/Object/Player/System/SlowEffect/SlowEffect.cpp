#include "SlowEffect.h"
#include "../../Player.h"

// 最大の大きさ
Vector2 SlowEffect::kMaxSize = { 5.0f, 5.0f };
// 最大の大きくなる速度
float SlowEffect::kMaxSizeVelocity = 1.0f;
// 最大の色の変更速度
float SlowEffect::kMaxColorVelocity = 0.1f;
// 大きくなる加速度
float SlowEffect::kSizeAcceleration_ = 0.01f;
// 色の変更加速度
float SlowEffect::kColorAcceleration_ = 0.01f;

void SlowEffect::Initalize(Player* player)
{

	// センター
	center_ = { 0.0f,0.0f };

	// 大きさ
	size_ = { 1.0f,1.0f };

	// 大きくなる速度
	sizeVelocity_ = 0.0f;

	// 色の媒介変数
	colorT_ = 0.0f;

	// 色の変更速度
	colorVelocity_ = 0.0f;

	// プレイヤー
	player_ = player;

	// カメラ
	camera_ = nullptr;
	
	// エフェクトを起動中か
	running_ = false;

	// 現在の状態
	state_ = kStateIndexSlowEnd;

}

void SlowEffect::Update()
{
	// 状態の変更
	preState_ = state_;
	if (player_->isSlowNow_) {
		state_ = kStateIndexSlowNow;
	}
	else {
		state_ = kStateIndexSlowEnd;
	}

	// 初期化
	if (preState_ == kStateIndexSlowEnd && state_ == kStateIndexSlowNow) {
		SetUp();
	}

	// エフェクト起動していない
	if (!running_) {
		return;
	}

	// 状態ごとの更新処理
	switch (state_)
	{
	case SlowEffect::kStateIndexSlowNow:
		SlowNowUpdate();
		break;
	case SlowEffect::kStateIndexSlowEnd:
		SlowEndUpdate();
		break;
	case SlowEffect::kStateIndexOfCount:
	default:
		assert(0);
		break;
	}

}

void SlowEffect::ImGuiDraw()
{

	ImGui::Text("\n");
	ImGui::SeparatorText("SlowEffect");

	// 最大の大きさ
	ImGui::DragFloat2("最大の大きさ", &kMaxSize.x, 0.01f, 0.01f);
	// 最大の大きくなる速度
	ImGui::DragFloat("最大の大きくなる速度", &kMaxSizeVelocity, 0.01f, 0.01f);
	// 最大の色の変更速度
	ImGui::DragFloat("最大の色の変更速度", &kMaxColorVelocity, 0.01f, 0.01f);
	// 大きくなる加速度
	ImGui::DragFloat("大きくなる加速度", &kSizeAcceleration_, 0.01f, 0.01f);
	// 色の変更加速度
	ImGui::DragFloat("色の変更加速度", &kColorAcceleration_, 0.01f, 0.01f);

}

void SlowEffect::SetUp()
{
	size_ = { 0.0f,0.0f };
	sizeVelocity_ = 0.0f;
	colorVelocity_ = 0.0f;
	// エフェクト起動
	running_ = true;
}

void SlowEffect::SlowNowUpdate()
{

	// 大きくなる速度
	sizeVelocity_ = std::clamp(sizeVelocity_ + kSizeAcceleration_, 0.0f, kMaxSizeVelocity);

	// 大きさ変更
	size_.x = std::clamp(size_.x + sizeVelocity_, 0.0f, kMaxSize.x);
	size_.y = std::clamp(size_.y + sizeVelocity_, 0.0f, kMaxSize.y);

	// 色速度
	colorVelocity_ = std::clamp(colorVelocity_ + kColorAcceleration_, 0.0f, kMaxColorVelocity);

	// 色変更
	colorT_ = std::clamp(colorT_ + colorVelocity_, 0.0f, 1.0f);

	// 中心設定
	center_ = MathUtility::ScreenPositionRatio(player_->worldtransform_.GetWorldPosition(), camera_);

}

void SlowEffect::SlowEndUpdate()
{

	if (colorT_ == 0.0f) {
		running_ = false;
		return;
	}

	// 色速度
	colorVelocity_ = std::clamp(colorVelocity_ + kColorAcceleration_, 0.0f, kMaxColorVelocity);

	// 色変更
	colorT_ = std::clamp(colorT_ - colorVelocity_, 0.0f, 1.0f);

	// 中心設定
	center_ = MathUtility::ScreenPositionRatio(player_->worldtransform_.GetWorldPosition(), camera_);

}
