#include "GameUIManager.h"
#include "../../../externals/imgui/imgui.h"
#include "../Object/Player/Player.h"
#include "../Object/Weapon/Weapon.h"
#include "../../../Engine/Math/Ease.h"

void GameUIManager::Initialze(ITextureHandleManager* texHandleManager)
{
	// インスタンスの取得
	dxCommon_		  = DirectXCommon::GetInstance(); // DirectX汎用クラス
	texHandleManager_ = texHandleManager;			  // テクスチャハンドルマネージャー
	input_			  = Input::GetInstance();		  // 入力

	// テクスチャロード開始
	LoadTexture();

	// スプライト生成
	CreateSprite();

	// 左スティックUIの座標を取得する
	stickUIPos_L_ = uiSprites_[LeftStickSprite]->GetPosition();
	stickUIPos_R_ = uiSprites_[RightStickSprite]->GetPosition();
}

void GameUIManager::Update()
{
	// クリア時、クリアUIの更新
	if (isClear_) {
		ClearUIUpdate();
	}
	else { // それ以外ではUIの更新
		// 左スティックUI更新
		LeftStickUIUpdate();

		// ジャンプボタンUI更新
		JumpButtonUIUpdate();

		// 右スティックUI更新
		RightStickUIUpdate();

		// 槍投げUIの更新
		ThrowButtonUIUpdate();

		// HPUIの更新
		HPUIUpdate();
	}
}

void GameUIManager::Draw()
{
	// 全スプライト分ループ
	for (int i = 0; i < spriteCount; i++) {
		// スプライト描画
		uiSprites_[i]->Draw();
	}
}

void GameUIManager::DisplayImGui()
{
	// ImGuiを開始
	ImGui::Begin("GameUIManager");
	// 全スプライト数分ループ
	for (int i = 0; i < spriteCount; i++) {
		// 名前取得
		std::string spriteName = "UI " + std::to_string(i);
		// ツリーノード表示
		if (ImGui::TreeNode(spriteName.c_str())) {
			// 編集中かつインデックス番号とiが一致している時
			if (isEditing_ && i == editingIndex_) {
				// ImGuiのスライダー表示
				ImGui::DragFloat2("Position", &forImGuiPosition_.x, 1.0f); // 位置
				ImGui::DragFloat2("Size", &forImGuiSize_.x, 1.0f); // 大きさ

				// 当該スプライトに値をセット
				uiSprites_[i]->SetPosition(forImGuiPosition_); // 位置
				uiSprites_[i]->SetSize(forImGuiSize_); // 位置

				// 編集終了
				if (ImGui::Button("Exit Edit")) {
					// 編集終了
					isEditing_ = false;
				}
			}
			else { // それ以外の場合
				// 各種情報取得
				Vector2 position = uiSprites_[i]->GetPosition(); // 位置
				Vector2 size = uiSprites_[i]->GetSize(); // 位置

				// ImGuiに表示
				ImGui::DragFloat2("Position", &position.x); // 位置
				ImGui::DragFloat2("Size", &size.x); // 位置

				// 編集中でない場合のみ実行可能
				if (!isEditing_) {
					// このボタンを押すと編集開始
					if (ImGui::Button("Edit")) {
						// 情報を取得しておく
						editingIndex_ = i;								  // インデックス取得
						forImGuiPosition_ = uiSprites_[i]->GetPosition(); // 座標
						forImGuiSize_ = uiSprites_[i]->GetSize();		  // スケール

						// 編集中フラグON
						isEditing_ = true;
					}
				}	
			}

			// ツリーノード終了
			ImGui::TreePop();
		}
	}

	ImGui::End();
}

void GameUIManager::LoadTexture()
{
	/// テクスチャロードを行う
	// 白テクスチャ
	texHandles_.insert({ White2x2Tex, TextureManager::Load("Resources/default/white2x2.png", dxCommon_, texHandleManager_) }); // 白
	// ボタン関係
	texHandles_.insert({ LeftStickNoneTex, TextureManager::Load("Resources/UI/Button/joystick_left_N.png", dxCommon_, texHandleManager_) }); // 左スティック入力無し
	texHandles_.insert({ LeftStickPressTex, TextureManager::Load("Resources/UI/Button/joystick_left_P.png", dxCommon_, texHandleManager_) }); // 左スティック入力あり
	texHandles_.insert({ AButtonNoneTex, TextureManager::Load("Resources/UI/Button/button_a_N.png", dxCommon_, texHandleManager_) });		 // Aボタン入力無し
	texHandles_.insert({ AButtonPressTex, TextureManager::Load("Resources/UI/Button/button_a_P.png", dxCommon_, texHandleManager_) });		 // Aボタン入力アリ
	texHandles_.insert({ LeftThumbNoneTex, TextureManager::Load("Resources/UI/Button/lb_N.png", dxCommon_, texHandleManager_) });			 // LBボタン入力無し
	texHandles_.insert({ LeftThumbPressTex, TextureManager::Load("Resources/UI/Button/lb_P.png", dxCommon_, texHandleManager_) });			 // LBボタン入力あり
	texHandles_.insert({ RightThumbNoneTex, TextureManager::Load("Resources/UI/Button/rb_N.png", dxCommon_, texHandleManager_) });			 // RBボタン入力なし
	texHandles_.insert({ RightThumbPressTex, TextureManager::Load("Resources/UI/Button/rb_P.png", dxCommon_, texHandleManager_) });			 // RBボタン入力あり
	texHandles_.insert({ RightStickNoneTex, TextureManager::Load("Resources/UI/Button/joystick_right_N.png", dxCommon_, texHandleManager_) });			 // 右スティック入力なし
	texHandles_.insert({ RightStickPressTex, TextureManager::Load("Resources/UI/Button/joystick_right_P.png", dxCommon_, texHandleManager_) });			 // 右スティック入力あり
	texHandles_.insert({ JoyStickBackTex, TextureManager::Load("Resources/UI/Button/joystick_Back.png", dxCommon_, texHandleManager_) });	 // スティック背景用

	// 画像関係
	texHandles_.insert({ DashImageTex, TextureManager::Load("Resources/UI/Game/dash.png", dxCommon_, texHandleManager_) });	 // ダッシュ画像
	texHandles_.insert({ JumpImageTex, TextureManager::Load("Resources/UI/Game/jump.png", dxCommon_, texHandleManager_) });	 // ジャンプ画像
	texHandles_.insert({ AimImageTex, TextureManager::Load("Resources/UI/Game/Aim.png", dxCommon_, texHandleManager_) });	 // エイム画像
	texHandles_.insert({ ThrowTextTex, TextureManager::Load("Resources/UI/Game/ThrowSpearText.png", dxCommon_, texHandleManager_) });	 // 投げるテキスト画像
	texHandles_.insert({ ReturnTextTex, TextureManager::Load("Resources/UI/Game/ReturnSpearText.png", dxCommon_, texHandleManager_) });	 // 戻るテキスト画像
	texHandles_.insert({ HPGageTex, TextureManager::Load("Resources/UI/Game/HealthGage.png", dxCommon_, texHandleManager_) });	 // HP画像
	texHandles_.insert({ HPGageFrameTex, TextureManager::Load("Resources/UI/Game/HealthGageFrame.png", dxCommon_, texHandleManager_) });	 // HPフレーム画像
}

void GameUIManager::CreateSprite()
{
	// セット用変数
	Vector2 setPosition; // 座標
	Vector2 setSize;	 // 大きさ

	/// スプライト生成
	// ボタン関係
	uiSprites_.push_back(std::move(std::make_unique<Sprite>())); // 左スティック背景用
	uiSprites_.back().reset(Sprite::Create(texHandles_[JoyStickBackTex], { 0.0f,0.0f }, { 1.0f,1.0f,1.0f,1.0f }));
	setPosition = { 80.0f, 525.0f };
	setSize = { 84.0f, 84.0f };
	uiSprites_.back()->SetPosition(setPosition);
	uiSprites_.back()->SetSize(setSize);

	uiSprites_.push_back(std::move(std::make_unique<Sprite>())); // 左スティック用
	uiSprites_.back().reset(Sprite::Create(texHandles_[LeftStickNoneTex], { 0.0f,0.0f }, { 1.0f,1.0f,1.0f,1.0f }));
	setPosition = { 80.0f, 525.0f };
	setSize = { 84.0f, 84.0f };
	uiSprites_.back()->SetPosition(setPosition);
	uiSprites_.back()->SetSize(setSize);

	uiSprites_.push_back(std::move(std::make_unique<Sprite>())); // ダッシュ画像用
	uiSprites_.back().reset(Sprite::Create(texHandles_[DashImageTex], { 0.0f,0.0f }, { 1.0f,1.0f,1.0f,1.0f }));
	setPosition = { 200.0f, 525.0f };
	setSize = { 96.0f, 96.0f };
	uiSprites_.back()->SetPosition(setPosition);
	uiSprites_.back()->SetSize(setSize);

	uiSprites_.push_back(std::move(std::make_unique<Sprite>())); // LBボタン用
	uiSprites_.back().reset(Sprite::Create(texHandles_[LeftThumbNoneTex], { 0.0f,0.0f }, { 1.0f,1.0f,1.0f,1.0f }));
	setPosition = { 80.0f, 645.0f };
	setSize = { 84.0f, 84.0f };
	uiSprites_.back()->SetPosition(setPosition);
	uiSprites_.back()->SetSize(setSize);

	uiSprites_.push_back(std::move(std::make_unique<Sprite>())); // ジャンプ画像用
	uiSprites_.back().reset(Sprite::Create(texHandles_[JumpImageTex], { 0.0f,0.0f }, { 1.0f,1.0f,1.0f,1.0f }));
	setPosition = { 200.0f, 645.0f };
	setSize = { 96.0f, 96.0f };
	uiSprites_.back()->SetPosition(setPosition);
	uiSprites_.back()->SetSize(setSize);

	uiSprites_.push_back(std::move(std::make_unique<Sprite>())); // 右スティック用
	uiSprites_.back().reset(Sprite::Create(texHandles_[JoyStickBackTex], { 0.0f,0.0f }, { 1.0f,1.0f,1.0f,1.0f }));
	setPosition = { 1050.0f, 525.0f };
	setSize = { 84.0f, 84.0f };
	uiSprites_.back()->SetPosition(setPosition);
	uiSprites_.back()->SetSize(setSize);

	uiSprites_.push_back(std::move(std::make_unique<Sprite>())); // 右スティック用
	uiSprites_.back().reset(Sprite::Create(texHandles_[RightStickNoneTex], { 0.0f,0.0f }, { 1.0f,1.0f,1.0f,1.0f }));
	setPosition = { 1050.0f, 525.0f };
	setSize = { 84.0f, 84.0f };
	uiSprites_.back()->SetPosition(setPosition);
	uiSprites_.back()->SetSize(setSize);

	uiSprites_.push_back(std::move(std::make_unique<Sprite>())); // エイム画像用
	uiSprites_.back().reset(Sprite::Create(texHandles_[AimImageTex], { 0.0f,0.0f }, { 1.0f,1.0f,1.0f,1.0f }));
	setPosition = { 1200.0f, 525.0f };
	setSize = { 96.0f, 96.0f };
	uiSprites_.back()->SetPosition(setPosition);
	uiSprites_.back()->SetSize(setSize);

	uiSprites_.push_back(std::move(std::make_unique<Sprite>())); // RBボタン用
	uiSprites_.back().reset(Sprite::Create(texHandles_[RightThumbNoneTex], { 0.0f,0.0f }, { 1.0f,1.0f,1.0f,1.0f }));
	setPosition = { 975.0f, 650.0f };
	setSize = { 84.0f, 84.0f };
	uiSprites_.back()->SetPosition(setPosition);
	uiSprites_.back()->SetSize(setSize);

	uiSprites_.push_back(std::move(std::make_unique<Sprite>())); // テキスト画像用
	uiSprites_.back().reset(Sprite::Create(texHandles_[ThrowTextTex], { 0.0f,0.0f }, { 1.0f,1.0f,1.0f,1.0f }));
	setPosition = { 1150.0f, 650.0f };
	setSize = { 256.0f, 64.0f };
	uiSprites_.back()->SetPosition(setPosition);
	uiSprites_.back()->SetSize(setSize);
	
	uiSprites_.push_back(std::move(std::make_unique<Sprite>())); // ゲージ用
	uiSprites_.back().reset(Sprite::Create(texHandles_[HPGageTex], { 0.0f,0.0f }, { 1.0f,1.0f,1.0f,1.0f }));
	setPosition = { 192.0f - (384.0f / 2.0f), 60.0f };
	setSize = { 384.0f, 64.0f };
	uiSprites_.back()->SetPosition(setPosition);
	uiSprites_.back()->SetSize(setSize);
	uiSprites_.back()->SetAnchorPoint({0.0f, 0.5f});

	uiSprites_.push_back(std::move(std::make_unique<Sprite>())); // ゲージフレーム用
	uiSprites_.back().reset(Sprite::Create(texHandles_[HPGageFrameTex], { 0.0f,0.0f }, { 1.0f,1.0f,1.0f,1.0f }));
	setPosition = { 192.0f, 60.0f };
	setSize = { 384.0f, 64.0f };
	uiSprites_.back()->SetPosition(setPosition);
	uiSprites_.back()->SetSize(setSize);

	uiSprites_.push_back(std::move(std::make_unique<Sprite>())); // クリア時背景
	uiSprites_.back().reset(Sprite::Create(texHandles_[White2x2Tex], { 0.0f,0.0f }, { 1.0f,1.0f,1.0f,1.0f }));
	setPosition = { 640.0f, 360.0f };
	setSize = { 1280.0f, 720.0f };
	uiSprites_.back()->SetPosition(setPosition);
	uiSprites_.back()->SetSize(setSize);
	uiSprites_.back()->SetColor({ 0.0f, 0.0f, 0.0f, 0.0f });

}

void GameUIManager::LeftStickUIUpdate()
{
	// スティック入力取得
	Vector2 stickVec = Vector2::Normalize(input_->GetLeftAnalogstick()) * stickUIOffset_;

	// スティック入力に応じてUIを動かす
	if (stickVec.x < 0.0f || stickVec.x > 0.0f || stickVec.y < 0.0f || stickVec.y > 0.0f) {
		uiSprites_[LeftStickSprite]->SetPosition(uiSprites_[LeftStickBackSprite]->GetPosition() + stickVec); // 座標を動かす
		uiSprites_[LeftStickSprite]->SetTextureHandle(texHandles_[LeftStickPressTex]);  // テクスチャ変更
	}
	else {
		uiSprites_[LeftStickSprite]->SetPosition(uiSprites_[LeftStickBackSprite]->GetPosition());			  // 元に戻す
		uiSprites_[LeftStickSprite]->SetTextureHandle(texHandles_[LeftStickNoneTex]);  // テクスチャ変更
	}
}

void GameUIManager::JumpButtonUIUpdate()
{
	/// ボタン入力に応じてUIを動かす
	// Aボタン
	//if (input_->PushJoystick(kJoystickButtonA)) {
	//	uiSprites_[AButtonSprite]->SetTextureHandle(texHandles_[AButtonPressTex]);  // テクスチャ変更
	//}
	//else {
	//	uiSprites_[AButtonSprite]->SetTextureHandle(texHandles_[AButtonNoneTex]);  // テクスチャ変更
	//}
	// LBボタン
	if (input_->PushJoystick(kJoystickButtonLB)) {
		uiSprites_[LBButtonSprite]->SetTextureHandle(texHandles_[LeftThumbPressTex]);  // テクスチャ変更
	}
	else {
		uiSprites_[LBButtonSprite]->SetTextureHandle(texHandles_[LeftThumbNoneTex]);  // テクスチャ変更
	}

	// 接地していない場合ジャンプ不可、灰色にする
	if (!isGrounded_) {
		jumpUIColor_ = { 0.5f, 0.5f, 0.5f, 1.0f };
	}
	else {
		jumpUIColor_ = { 1.0f, 1.0f, 1.0f, 1.0f };
	}
	//uiSprites_[AButtonSprite]->SetColor(jumpUIColor_);
	uiSprites_[LBButtonSprite]->SetColor(jumpUIColor_);
	uiSprites_[JumpSprite]->SetColor(jumpUIColor_);
}

void GameUIManager::RightStickUIUpdate()
{
	// スティック入力取得
	Vector2 stickVec = Vector2::Normalize(input_->GetRightAnalogstick()) * stickUIOffset_;

	// スティック入力に応じてUIを動かす
	if (stickVec.x < 0.0f || stickVec.x > 0.0f || stickVec.y < 0.0f || stickVec.y > 0.0f) {
		uiSprites_[RightStickSprite]->SetPosition(uiSprites_[RightStickBackSprite]->GetPosition() + stickVec); // 座標を動かす
		uiSprites_[RightStickSprite]->SetTextureHandle(texHandles_[RightStickPressTex]);  // テクスチャ変更
	}
	else {
		uiSprites_[RightStickSprite]->SetPosition(uiSprites_[RightStickBackSprite]->GetPosition());					    // 元に戻す
		uiSprites_[RightStickSprite]->SetTextureHandle(texHandles_[RightStickNoneTex]);  // テクスチャ変更
	}

	if (!player_->weapon_->isHold_) {
		rStickUIColor_ = { 0.5f, 0.5f, 0.5f, 1.0f };
	}
	else {
		rStickUIColor_ = { 1.0f, 1.0f, 1.0f, 1.0f };
	}


	uiSprites_[RightStickBackSprite]->SetColor(rStickUIColor_);
	uiSprites_[RightStickSprite]->SetColor(rStickUIColor_);
	uiSprites_[AimSprite]->SetColor(rStickUIColor_);
}

void GameUIManager::ThrowButtonUIUpdate()
{
	/// ボタン入力に応じてUIを動かす
	// RBボタン
	if (input_->PushJoystick(kJoystickButtonRB)) {
		uiSprites_[RBButtonSprite]->SetTextureHandle(texHandles_[RightThumbPressTex]);  // テクスチャ変更
	}
	else {
		uiSprites_[RBButtonSprite]->SetTextureHandle(texHandles_[RightThumbNoneTex]);  // テクスチャ変更
	}

	if (player_->weapon_->isHold_) {
		uiSprites_[TextSprite]->SetTextureHandle(texHandles_[ThrowTextTex]);  // テクスチャ変更
	}
	else {
		uiSprites_[TextSprite]->SetTextureHandle(texHandles_[ReturnTextTex]);  // テクスチャ変更
	}

	// 槍を所持していない時に灰色に
	if (std::holds_alternative<ThrownState*>(player_->weapon_->GetNowState())) {
		throwUIColor_ = { 0.5f, 0.5f, 0.5f, 1.0f };
	}
	else {
		throwUIColor_ = { 1.0f, 1.0f, 1.0f, 1.0f };
	}
	uiSprites_[RBButtonSprite]->SetColor(throwUIColor_);
	uiSprites_[TextSprite]->SetColor(throwUIColor_);
}

void GameUIManager::HPUIUpdate()
{
	// 一時変数
	float size = 0.0f;
	float texSize = 0.0f;

	if (player_->GetCurrentHealth() >= 0) {
		switch (player_->GetCurrentHealth())
		{
		case 0:
			texSize = Ease::Easing(Ease::EaseName::Lerp, uiSprites_[HPGageSprite]->GetTextureSize().x, 0.0f, 0.25f);
			size = Ease::Easing(Ease::EaseName::Lerp, uiSprites_[HPGageSprite]->GetSize().x, 0.0f, 0.25f);
			break;
		case 1:
			texSize = Ease::Easing(Ease::EaseName::Lerp, uiSprites_[HPGageSprite]->GetTextureSize().x, 394.0f, 0.25f);
			size = Ease::Easing(Ease::EaseName::Lerp, uiSprites_[HPGageSprite]->GetSize().x, 138.0f, 0.25f);
			break;
		case 2:
			texSize = Ease::Easing(Ease::EaseName::Lerp, uiSprites_[HPGageSprite]->GetTextureSize().x, 768.0f, 0.25f);
			size = Ease::Easing(Ease::EaseName::Lerp, uiSprites_[HPGageSprite]->GetSize().x, 256.0f, 0.25f);
			break;
		case 3:
			texSize = Ease::Easing(Ease::EaseName::Lerp, uiSprites_[HPGageSprite]->GetTextureSize().x, 1152.0f, 0.25f);
			size = Ease::Easing(Ease::EaseName::Lerp, uiSprites_[HPGageSprite]->GetSize().x, 384.0f, 0.25f);
			break;
		}
	}

	uiSprites_[HPGageSprite]->SetTextureSize({ texSize, 192.0f });
	uiSprites_[HPGageSprite]->SetSize({ size, 64.0f });
}

void GameUIManager::ClearUIUpdate()
{
	// クリア文字が出現しきってなかった場合
	if (!isClearAppear_) {
		if (currentClearAppearTime_ < clearAppearTime_) {
			// 線形補間で演出を行う
			float alpha = Ease::Easing(Ease::EaseName::EaseOutQuad, 0.0f, 0.75f, currentClearAppearTime_ / clearAppearTime_);
			float uiAlpha = Ease::Easing(Ease::EaseName::EaseOutQuad, 1.0f, 0.0f, currentClearAppearTime_ / clearAppearTime_);

			// 全スプライト分ループ
			for (int i = 0; i < spriteCount; i++) {
				// UIを透明に
				Vector4 prevColor = uiSprites_[i]->GetColor();
				uiSprites_[i]->SetColor({ prevColor.x, prevColor.y, prevColor.z, uiAlpha });
			}
			uiSprites_[ClearBackFrameSprite]->SetColor({ 0.0f, 0.0f, 0.0f, alpha });
			// 現在時間を加算
			currentClearAppearTime_ += kDeltaTime_;
		}
		else {
			// 終了時一定値で固定
			uiSprites_[ClearBackFrameSprite]->SetColor({ 0.0f, 0.0f, 0.0f, 0.75f });

			// クリア演出終了
			isClearAppear_ = true;
		}
	}
}
