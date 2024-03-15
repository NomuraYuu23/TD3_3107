#include "Spear.h"
#include "../../../Engine/2D/ImguiManager.h"

void Spear::Initialize(Model* model)
{
	// 基底クラスの初期化処理
	IObject::Initialize(model);

	// ワールドトランスフォームの初期化
	worldtransform_.Initialize(model_->GetRootNode());

	/// ローカル座標関係の初期化
	// 座標
	std::vector<Vector3> initPositions;
	for (size_t i = 0; i < worldtransform_.GetNodeDatas().size(); i++) {
		// 新規座標生成
		Vector3 newPosition = { 0.0f, 0.0f, 0.0f };
		// 生成した座標を配列に入れる
		initPositions.push_back(newPosition);
	}
	// クォータニオン
	std::vector<Quaternion> initQuaternions;
	for (size_t i = 0; i < worldtransform_.GetNodeDatas().size(); i++) {
		// 新規クォータニオン生成
		Quaternion newQuaternion = { 0.0f, 0.0f, 0.0f, 1.0f };
		// 生成したクォータニオンを配列に入れる
		initQuaternions.push_back(newQuaternion);
	}
	// スケール
	std::vector<Vector3> initScales;
	for (size_t i = 0; i < worldtransform_.GetNodeDatas().size(); i++) {
		// 新規スケール生成
		Vector3 newScale = { 1.0f, 1.0f, 1.0f };
		// 生成した座標を配列に入れる
		initScales.push_back(newScale);
	}

	// アニメーション生成
	anim_ = std::make_unique<Animation>();
	// アニメーション初期化
	anim_->Initialize(
		model_->GetNodeAnimationData(),
		initPositions,
		initQuaternions,
		initScales,
		worldtransform_.GetNodeNames()
	);

	// アニメーション再生
	anim_->startAnimation(0, true);
}

void Spear::Update()
{
	// アニメーション更新
	worldtransform_.SetNodeLocalMatrix(anim_->AnimationUpdate());

	worldtransform_.UpdateMatrix();

	// ImGui描画
	ImGuiDraw();
}

void Spear::Draw(BaseCamera camera)
{
	// 描画処理
	model_->Draw(worldtransform_, camera);
}

void Spear::ImGuiDraw()
{
	// ImGui開始
	ImGui::Begin("Spear");

	// トランスフォームの情報表示
	ImGui::DragFloat3("Scale", &worldtransform_.transform_.scale.x, 0.001f, 0.0f); // 拡縮
	ImGui::DragFloat3("Rotate", &worldtransform_.transform_.rotate.x, 0.001f);	   // 回転
	ImGui::DragFloat3("Translate", &worldtransform_.transform_.translate.x, 0.1f); // 位置

	// ImGui終了
	ImGui::End();
}

void Spear::OnCollision(ColliderParentObject2D target)
{
	target;
}
