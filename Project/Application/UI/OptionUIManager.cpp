#include "OptionUIManager.h"
#include "../../../externals/imgui/imgui.h"
#include "../Object/Player/Player.h"
#include "../Object/Weapon/Weapon.h"
#include "../../../Engine/Math/Ease.h"

int OptionUIManager::SelectedControlType_ = OptionUIManager::Control_RBPressThrow;
int OptionUIManager::SelectedAimAssistType_ = OptionUIManager::Assist_Medium;

void OptionUIManager::Initialze()
{
	// インスタンスの取得
	dxCommon_ = DirectXCommon::GetInstance(); // DirectX汎用クラス
	input_ = Input::GetInstance();		  // 入力

	// テクスチャロード開始
	LoadTexture();

	// スプライト生成
	CreateSprite();

	// 事前座標を取得
	categoryTextPos_ = uiSprites_[OptionControlTextSprite]->GetPosition();
	aimAssistTextPos_ = uiSprites_[OptionAimAssistSprite]->GetPosition();
}

void OptionUIManager::Update()
{
	// カテゴリ選択中でない時
	if (input_->TriggerJoystick(kJoystickButtonB) && !isCategorySelected_) {
		// オプションを描画しない
		isDraw_ = false;
	}

	// カテゴリ関連更新
	CategoryUpdate();
}

void OptionUIManager::Draw()
{
	// 全スプライト分ループ
	for (int i = 0; i < OSpriteCount; i++) {
		// スプライト描画
		uiSprites_[i]->Draw();
	}
}

void OptionUIManager::DisplayImGui()
{
	// ImGuiを開始
	ImGui::Begin("OptionUIManager");
	// 全スプライト数分ループ
	for (int i = 0; i < OSpriteCount; i++) {
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


void OptionUIManager::SetPlayer(Player* player)
{
	// プレイヤー取得
	player_ = player;

	player_->GetController()->throwType_ = SelectedControlType_;
	player_->GetCorrectSystem().assistLevel_ = SelectedAimAssistType_;
}

void OptionUIManager::SetIsDraw(const bool isDraw) {
	// フラグ取得
	isDraw_ = isDraw;

	// 選択リセット
	isCategorySelected_ = false;
}

void OptionUIManager::LoadTexture()
{
	/// テクスチャロードを行う
	// オプションテキスト
	texHandles_.insert({ OptionTextTex, TextureManager::Load("Resources/UI/Game/OptionTextTex.png", dxCommon_) }); // オプションテキスト
	texHandles_.insert({ CursorTex, TextureManager::Load("Resources/UI/Game/PoseCusorTex.png", dxCommon_) }); // カーソル
	// 操作カテゴリ系
	texHandles_.insert({ ControlTextTex, TextureManager::Load("Resources/UI/Game/OptionControlTextTex.png", dxCommon_) }); // 操作テキスト
	texHandles_.insert({ ChangeControlTypeTex, TextureManager::Load("Resources/UI/Game/ChangeControlType.png", dxCommon_) }); // 操作テキスト
	texHandles_.insert({ RightArrowTex, TextureManager::Load("Resources/UI/Game/RightArrow.png", dxCommon_) }); // 操作テキスト
	texHandles_.insert({ LeftArrowTex, TextureManager::Load("Resources/UI/Game/LeftArrow.png", dxCommon_) }); // 操作テキスト
	texHandles_.insert({ ThrowControlsTex, TextureManager::Load("Resources/UI/Game/ThrowControlsUI.png", dxCommon_) }); // 操作テキスト
	// エイムアシスト系
	texHandles_.insert({ AimAssistTextTex, TextureManager::Load("Resources/UI/Game/AimAssistTextTex.png", dxCommon_) }); // エイムアシストテキスト
	texHandles_.insert({ AimStrengthTex, TextureManager::Load("Resources/UI/Game/AimAssistStrength.png", dxCommon_) }); // エイムアシストテキスト

}

void OptionUIManager::CreateSprite()
{
	// セット用変数
	Vector2 setPosition; // 座標
	Vector2 setSize;	 // 大きさ

#pragma region 大元UI

	uiSprites_.push_back(std::move(std::make_unique<Sprite>())); // オプションテキスト
	uiSprites_.back().reset(Sprite::Create(texHandles_[OptionTextTex], { 0.0f,0.0f }, { 1.0f,1.0f,1.0f,1.0f }));
	setPosition = { 640.0f, 125.0f };
	setSize = { 540.0f, 192.0f };
	uiSprites_.back()->SetPosition(setPosition);
	uiSprites_.back()->SetSize(setSize);

	uiSprites_.push_back(std::move(std::make_unique<Sprite>())); // カーソル
	uiSprites_.back().reset(Sprite::Create(texHandles_[CursorTex], { 0.0f,0.0f }, { 1.0f,1.0f,1.0f,1.0f }));
	setPosition = { 250.0f, 350.0f };
	setSize = { 64.0f, 64.0f };
	uiSprites_.back()->SetPosition(setPosition);
	uiSprites_.back()->SetSize(setSize);

#pragma endregion

#pragma region カテゴリ別UI

	uiSprites_.push_back(std::move(std::make_unique<Sprite>())); // 操作カテゴリ
	uiSprites_.back().reset(Sprite::Create(texHandles_[ControlTextTex], { 0.0f,0.0f }, { 1.0f,1.0f,1.0f,1.0f }));
	setPosition = { 640.0f, 350.0f };
	setSize = { 720.0f, 128.0f };
	uiSprites_.back()->SetPosition(setPosition);
	uiSprites_.back()->SetSize(setSize);

	uiSprites_.push_back(std::move(std::make_unique<Sprite>())); // エイムアシストカテゴリ
	uiSprites_.back().reset(Sprite::Create(texHandles_[AimAssistTextTex], { 0.0f,0.0f }, { 1.0f,1.0f,1.0f,1.0f }));
	setPosition = { 640.0f, 550.0f };
	setSize = { 720.0f, 128.0f };
	uiSprites_.back()->SetPosition(setPosition);
	uiSprites_.back()->SetSize(setSize);

#pragma endregion

#pragma region 選択したカテゴリの表示UI

	uiSprites_.push_back(std::move(std::make_unique<Sprite>())); // 選択中カテゴリUI
	uiSprites_.back().reset(Sprite::Create(texHandles_[ThrowControlsTex], { 0.0f,0.0f }, { 1.0f,1.0f,1.0f,1.0f }));
	setPosition = { 640.0f, 550.0f };
	setSize = { 720.0f, 96.0f };
	uiSprites_.back()->SetPosition(setPosition);
	uiSprites_.back()->SetTextureSize({720.0f, 96.0f});
	uiSprites_.back()->SetSize(setSize);
	uiSprites_.back()->SetColor({ 1.0f, 1.0f, 1.0f, 0.0f });

	uiSprites_.push_back(std::move(std::make_unique<Sprite>())); // 右カーソル
	uiSprites_.back().reset(Sprite::Create(texHandles_[RightArrowTex], { 0.0f,0.0f }, { 1.0f,1.0f,1.0f,1.0f }));
	setPosition = { 1025.0f, 550.0f };
	setSize = { 64.0f, 64.0f };
	uiSprites_.back()->SetPosition(setPosition);
	uiSprites_.back()->SetSize(setSize);
	uiSprites_.back()->SetColor({ 1.0f, 1.0f, 1.0f, 0.0f });

	uiSprites_.push_back(std::move(std::make_unique<Sprite>())); // 左カーソル
	uiSprites_.back().reset(Sprite::Create(texHandles_[LeftArrowTex], { 0.0f,0.0f }, { 1.0f,1.0f,1.0f,1.0f }));
	setPosition = { 250.0f, 550.0f };
	setSize = { 64.0f, 64.0f };
	uiSprites_.back()->SetPosition(setPosition);
	uiSprites_.back()->SetSize(setSize);
	uiSprites_.back()->SetColor({ 1.0f, 1.0f, 1.0f, 0.0f });

#pragma endregion

}

void OptionUIManager::CategoryUpdate()
{
	// 左スティックの入力取得
	Vector2 leftStick = input_->GetLeftAnalogstick();

	if (currentCategoryCursorCoolTime_ > categoryCursorCoolTime_) {
		if (!isCategorySelected_) { // カテゴリ選択されている状態でなければ
			// 選択項目を上に
			if (leftStick.y < -0.5f) {

				player_->gameAudioManager_->PlayWave(kCurosrSE);

				if (SelectedCategory_ == Category_Control) {
					SelectedCategory_ = Category_AimAssist;
				}
				else {
					SelectedCategory_--;
				}
				currentCategoryCursorCoolTime_ = 0.0f;
			}

			// 選択項目を下に
			if (leftStick.y > 0.5f) {

				player_->gameAudioManager_->PlayWave(kCurosrSE);

				if (SelectedCategory_ == Category_AimAssist) {
					SelectedCategory_ = Category_Control;
				}
				else {
					SelectedCategory_++;
				}
				currentCategoryCursorCoolTime_ = 0.0f;
			}
		}
	}
	else {
		// 経過秒数加算
		currentCategoryCursorCoolTime_ += kDeltaTime_;

		// 入力が無ければクールタイムリセット
		if (leftStick.x == 0.0f && leftStick.y == 0.0f) {
			currentCategoryCursorCoolTime_ = 10.0f;
		}

	}

	// カーソルY座標
	float cursorPosY = 0.0f;

	// 選択されているカテゴリに酔って分岐
	switch (SelectedCategory_)
	{
	case Category_Control: // 操作系UI選択時
		if (isCategorySelected_) { // 選択されている場合

			// 操作カテゴリ系オプションの更新関数
			ControlCategoryUpdate();

			if (input_->TriggerJoystick(kJoystickButtonB)) {
				// 選択していない状態に
				isCategorySelected_ = false;
				player_->GetController()->throwType_ = SelectedControlType_;
			}
		}
		else {
			if (input_->TriggerJoystick(kJoystickButtonA)) {
				
				player_->gameAudioManager_->PlayWave(kEnterSE);
				
				// 選択されたUIの透明度を1に
				uiSprites_[OptionControlTextSprite]->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
				// 選択状態に
				isCategorySelected_ = true;

				uiSprites_[OptionSelectedCategoryUI]->SetTextureHandle(texHandles_[ThrowControlsTex]);
			}
		}

		// カーソルを移動させる
		cursorPosY = Ease::Easing(Ease::EaseName::EaseOutQuad, uiSprites_[OptionCategoryCursorSprite]->GetPosition().y, uiSprites_[OptionControlTextSprite]->GetPosition().y, 0.2f);

		break;
	case Category_AimAssist: // エイムアシスト系選択時
		if (isCategorySelected_) { // 選択されている場合

			// 操作カテゴリ系オプションの更新関数
			AimAssistCategoryUpdate();

			if (input_->TriggerJoystick(kJoystickButtonB)) {
				// 選択していない状態に
				isCategorySelected_ = false;
				player_->GetCorrectSystem().assistLevel_ = SelectedAimAssistType_;
			}
		}
		else {
			if (input_->TriggerJoystick(kJoystickButtonA)) {
				// 選択されたUIの透明度を1に
				uiSprites_[OptionAimAssistSprite]->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
				// 選択状態に
				isCategorySelected_ = true;

				uiSprites_[OptionSelectedCategoryUI]->SetTextureHandle(texHandles_[AimStrengthTex]);
			}
		}

		// カーソルを移動させる
		cursorPosY = Ease::Easing(Ease::EaseName::EaseOutQuad, uiSprites_[OptionCategoryCursorSprite]->GetPosition().y, uiSprites_[OptionAimAssistSprite]->GetPosition().y, 0.2f);

		break;
	}

	// カーソルを移動させる
	uiSprites_[OptionCategoryCursorSprite]->SetPosition({ uiSprites_[OptionCategoryCursorSprite]->GetPosition().x, cursorPosY });

	// カテゴリが選択されていない場合、カテゴリスプライトの透明度をいじる
	if (!isCategorySelected_) {
		// カテゴリ透明度制御
		uiSprites_[OptionControlTextSprite]->SetColor({ 1.0f, 1.0f, 1.0f, Ease::Easing(Ease::EaseName::EaseOutQuad, uiSprites_[OptionControlTextSprite]->GetColor().w, 1.0f, 0.2f)});
		uiSprites_[OptionAimAssistSprite]->SetColor({ 1.0f, 1.0f, 1.0f, Ease::Easing(Ease::EaseName::EaseOutQuad, uiSprites_[OptionAimAssistSprite]->GetColor().w, 1.0f, 0.2f)});

		// カテゴリ位置制御
		uiSprites_[OptionControlTextSprite]->SetPosition(Ease::Easing(Ease::EaseName::EaseOutQuad, uiSprites_[OptionControlTextSprite]->GetPosition(), categoryTextPos_, 0.2f));
		uiSprites_[OptionAimAssistSprite]->SetPosition(Ease::Easing(Ease::EaseName::EaseOutQuad, uiSprites_[OptionAimAssistSprite]->GetPosition(), aimAssistTextPos_, 0.2f));

		// 選択カテゴリに透明度制御
		//uiSprites_[OptionSelectedCategoryText]->SetColor({ 1.0f, 1.0f, 1.0f, Ease::Easing(Ease::EaseName::EaseOutQuad, uiSprites_[OptionSelectedCategoryText]->GetColor().w, 0.0f, 0.2f) });
		uiSprites_[OptionSelectedCategoryUI]->SetColor({ 1.0f, 1.0f, 1.0f, Ease::Easing(Ease::EaseName::EaseOutQuad, uiSprites_[OptionSelectedCategoryUI]->GetColor().w, 0.0f, 0.2f) });
		uiSprites_[Control_RightArrowSprite]->SetColor({ 1.0f, 1.0f, 1.0f, Ease::Easing(Ease::EaseName::EaseOutQuad, uiSprites_[Control_RightArrowSprite]->GetColor().w, 0.0f, 0.2f) });
		uiSprites_[Control_LeftArrowSprite]->SetColor({ 1.0f, 1.0f, 1.0f, Ease::Easing(Ease::EaseName::EaseOutQuad, uiSprites_[Control_LeftArrowSprite]->GetColor().w, 0.0f, 0.2f) });
	}
}

void OptionUIManager::ControlCategoryUpdate()
{
	// 他のカテゴリスプライトの透明度を設定
	uiSprites_[OptionAimAssistSprite]->SetColor({ 1.0f, 1.0f, 1.0f, Ease::Easing(Ease::EaseName::EaseOutQuad, uiSprites_[OptionAimAssistSprite]->GetColor().w, 0.0f, 0.2f) });

	// 選択カテゴリスプライトを動作させる
	uiSprites_[OptionControlTextSprite]->SetPosition(Ease::Easing(Ease::EaseName::EaseOutQuad, uiSprites_[OptionControlTextSprite]->GetPosition(), { uiSprites_[OptionControlTextSprite]->GetPosition().x , 300.0f }, 0.2f));

	// 選択カテゴリに関するUIを表示
	//uiSprites_[OptionSelectedCategoryText]->SetColor({ 1.0f, 1.0f, 1.0f, Ease::Easing(Ease::EaseName::EaseOutQuad, uiSprites_[OptionSelectedCategoryText]->GetColor().w, 1.0f, 0.2f) });
	uiSprites_[OptionSelectedCategoryUI]->SetColor({ 1.0f, 1.0f, 1.0f, Ease::Easing(Ease::EaseName::EaseOutQuad, uiSprites_[OptionSelectedCategoryUI]->GetColor().w, 1.0f, 0.2f) });
	uiSprites_[Control_RightArrowSprite]->SetColor({ 1.0f, 1.0f, 1.0f, Ease::Easing(Ease::EaseName::EaseOutQuad, uiSprites_[Control_RightArrowSprite]->GetColor().w, 1.0f, 0.2f) });
	uiSprites_[Control_LeftArrowSprite]->SetColor({ 1.0f, 1.0f, 1.0f, Ease::Easing(Ease::EaseName::EaseOutQuad, uiSprites_[Control_LeftArrowSprite]->GetColor().w, 1.0f, 0.2f) });

	// 左スティックの入力取得
	Vector2 leftStick = input_->GetLeftAnalogstick();

	if (currentCategoryCursorCoolTime_ > categoryCursorCoolTime_) {
		// 選択項目を上に
		if (leftStick.x < -0.5f) {

			player_->gameAudioManager_->PlayWave(kLeftSE);

			if (SelectedControlType_ == Control_RBPressThrow) {
				SelectedControlType_ = Control_RStickReleaseThrow;
			}
			else {
				SelectedControlType_--;
			}
			currentCategoryCursorCoolTime_ = 0.0f;
		}

		// 選択項目を下に
		if (leftStick.x > 0.5f) {

			player_->gameAudioManager_->PlayWave(kRightSE);

			if (SelectedControlType_ == Control_RStickReleaseThrow) {
				SelectedControlType_ = Control_RBPressThrow;
			}
			else {
				SelectedControlType_++;
			}
			currentCategoryCursorCoolTime_ = 0.0f;
		}
	}

	float range;

	switch (SelectedControlType_)
	{
	case Control_RBPressThrow: // RBをプッシュした時に投げる
		range = Ease::Easing(Ease::EaseName::EaseOutQuad, uiSprites_[OptionSelectedCategoryUI]->GetTextureLeftTop().x, 0.0f, 0.3f);
		break;
	case Control_RBReleaseThrow: // RBを離した時に投げる
		range = Ease::Easing(Ease::EaseName::EaseOutQuad, uiSprites_[OptionSelectedCategoryUI]->GetTextureLeftTop().x, 720.0f, 0.3f);
		break;
	case Control_RStickReleaseThrow: // スティックを離した時に投げる
		range = Ease::Easing(Ease::EaseName::EaseOutQuad, uiSprites_[OptionSelectedCategoryUI]->GetTextureLeftTop().x, 1440.0f, 0.3f);
		break;
	}

	uiSprites_[OptionSelectedCategoryUI]->SetTextureLeftTop({ range, 0.0f });

}

void OptionUIManager::AimAssistCategoryUpdate()
{
	// 他のカテゴリスプライトの透明度を設定
	uiSprites_[OptionControlTextSprite]->SetColor({ 1.0f, 1.0f, 1.0f, Ease::Easing(Ease::EaseName::EaseOutQuad, uiSprites_[OptionControlTextSprite]->GetColor().w, 0.0f, 0.2f) });
	
	// 選択カテゴリスプライトを動作させる
	uiSprites_[OptionAimAssistSprite]->SetPosition(Ease::Easing(Ease::EaseName::EaseOutQuad, uiSprites_[OptionAimAssistSprite]->GetPosition(), { uiSprites_[OptionAimAssistSprite]->GetPosition().x , 300.0f }, 0.2f));

	// 選択カテゴリに関するUIを表示
	//uiSprites_[OptionSelectedCategoryText]->SetColor({ 1.0f, 1.0f, 1.0f, Ease::Easing(Ease::EaseName::EaseOutQuad, uiSprites_[OptionSelectedCategoryText]->GetColor().w, 1.0f, 0.2f) });
	uiSprites_[OptionSelectedCategoryUI]->SetColor({ 1.0f, 1.0f, 1.0f, Ease::Easing(Ease::EaseName::EaseOutQuad, uiSprites_[OptionSelectedCategoryUI]->GetColor().w, 1.0f, 0.2f) });
	uiSprites_[Control_RightArrowSprite]->SetColor({ 1.0f, 1.0f, 1.0f, Ease::Easing(Ease::EaseName::EaseOutQuad, uiSprites_[Control_RightArrowSprite]->GetColor().w, 1.0f, 0.2f) });
	uiSprites_[Control_LeftArrowSprite]->SetColor({ 1.0f, 1.0f, 1.0f, Ease::Easing(Ease::EaseName::EaseOutQuad, uiSprites_[Control_LeftArrowSprite]->GetColor().w, 1.0f, 0.2f) });

	// 左スティックの入力取得
	Vector2 leftStick = input_->GetLeftAnalogstick();

	if (currentCategoryCursorCoolTime_ > categoryCursorCoolTime_) {
		// 選択項目を上に
		if (leftStick.x < -0.5f) {

			player_->gameAudioManager_->PlayWave(kLeftSE);

			if (SelectedAimAssistType_ == Assist_Low) {
				SelectedAimAssistType_ = Assist_High;
			}
			else {
				SelectedAimAssistType_--;
			}
			currentCategoryCursorCoolTime_ = 0.0f;
		}

		// 選択項目を下に
		if (leftStick.x > 0.5f) {

			player_->gameAudioManager_->PlayWave(kRightSE);

			if (SelectedAimAssistType_ == Assist_High) {
				SelectedAimAssistType_ = Assist_Low;
			}
			else {
				SelectedAimAssistType_++;
			}
			currentCategoryCursorCoolTime_ = 0.0f;
		}
	}

	float range;

	switch (SelectedAimAssistType_)
	{
	case Control_RBPressThrow: // RBをプッシュした時に投げる
		range = Ease::Easing(Ease::EaseName::EaseOutQuad, uiSprites_[OptionSelectedCategoryUI]->GetTextureLeftTop().x, 0.0f, 0.3f);
		break;
	case Control_RBReleaseThrow: // RBを離した時に投げる
		range = Ease::Easing(Ease::EaseName::EaseOutQuad, uiSprites_[OptionSelectedCategoryUI]->GetTextureLeftTop().x, 720.0f, 0.3f);
		break;
	case Control_RStickReleaseThrow: // スティックを離した時に投げる
		range = Ease::Easing(Ease::EaseName::EaseOutQuad, uiSprites_[OptionSelectedCategoryUI]->GetTextureLeftTop().x, 1440.0f, 0.3f);
		break;
	}

	uiSprites_[OptionSelectedCategoryUI]->SetTextureLeftTop({ range, 0.0f });
}
