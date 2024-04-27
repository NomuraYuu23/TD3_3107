#include "PlayerFootCollider.h"
#include "../ObjectList.h"
#include "../../Collider2D/CollisionConfig2D.h"

void PlayerFootCollider::Initialize(Model* model, Player* parent)
{
	
	model_ = model;
	player_ = parent;

	// マテリアル
	material_.reset(Material::Create());

	// 行列
	worldtransform_.Initialize(model_->GetRootNode());
	worldtransform_.UpdateMatrix();

	// 2D用座標・サイズ
	position2D_ = { worldtransform_.GetWorldPosition().x,worldtransform_.GetWorldPosition().y };
	//scale2D_ = { 0.95f*1.95f, 0.2f };
	scale2D_ = { 1.7f, 0.2f };

	worldtransform_.SetParent(&player_->worldtransform_);
	worldtransform_.transform_.translate.y = -(player_->circleCollider_.radius_ + 0.05f);

	// コライダー
	boxCollider_.Initialize(position2D_, scale2D_.x, scale2D_.y, 0, this);
	boxCollider_.SetCollisionAttribute(kCollisionAttributePlayer);
	boxCollider_.SetCollisionMask(kCollisionAttributeTerrain);

	// ローカル行列マネージャー
	localMatrixManager_ = std::make_unique<LocalMatrixManager>();
	localMatrixManager_->Initialize(model_->GetRootNode());

}

void PlayerFootCollider::Update()
{
	// 行列・座標更新
	worldtransform_.UpdateMatrix();
	position2D_ = { worldtransform_.GetWorldPosition().x,worldtransform_.GetWorldPosition().y};
	boxCollider_.Update(position2D_, scale2D_.x, scale2D_.y, 0);

	// 接地フラグの初期化
	player_->isGround_ = false;

}

void PlayerFootCollider::ImGuiDraw()
{

	ImGui::DragFloat3("footPos", &worldtransform_.transform_.translate.x);
	ImGui::DragFloat3("footScale", &worldtransform_.transform_.scale.x);
	ImGui::DragFloat2("coPos", &position2D_.x);
	ImGui::DragFloat2("scale2D", &scale2D_.x);

}

void PlayerFootCollider::OnCollision(ColliderParentObject2D target)
{
	if (!std::holds_alternative<Terrain*>(target)) {
		return;
	}
	else {
		Vector2 targetPos = {};
		Vector2 targetRad = {};
		// 対象の情報取得
		std::visit([&](const auto& a) {
			targetPos = a->GetColliderPosition();
			targetRad = a->GetColliderSize();
			}, target);
		targetRad *= 0.5f;
		// 右上
		Vector3 maxPos = {
			targetPos.x + targetRad.x,	// 右
			targetPos.y + targetRad.y,	// 上
		};
		// 左下
		Vector3 minPos = {
			targetPos.x - targetRad.x,	// 左
			targetPos.y - targetRad.y,	// 下
		};
		Vector3 lerpPos = worldtransform_.GetWorldPosition();
		Vector2 plMin = { lerpPos.x - scale2D_.x,lerpPos.y - scale2D_.y };
		Vector2 plMax = { lerpPos.x + scale2D_.x,lerpPos.y + scale2D_.y };

		// 四頂点
		IObject::FourTop player4Point = IObject::GenerateFourTop(plMin, plMax);
		IObject::CollisionType type = IObject::GetCollisionType(player4Point, { minPos.x,minPos.y }, { maxPos.x,maxPos.y });

		// コンボリセット
		if (std::holds_alternative<GroundState*>(player_->GetNowState())) {
			player_->ResetCombo();
		}
		// 着地フラグ
		if (type == IObject::kBottomSide || type == IObject::kRBPoint || type == IObject::kLBPoint) {
			player_->isGround_ = true;
		}
	}

}

void PlayerFootCollider::DebugDraw(BaseCamera camera)
{

	ModelDraw::AnimObjectDesc desc;
	desc.camera = &const_cast<BaseCamera&>(camera);
	desc.localMatrixManager = localMatrixManager_.get();
	desc.material = material_.get();
	desc.model = model_;
	desc.worldTransform = &worldtransform_;
	ModelDraw::AnimObjectDraw(desc);

}
