#include "IEnemyEmitter.h"
#include "Enemy.h"
#include "../../../Engine/2D/ImguiManager.h"
#include "../../../Engine/base/SRVDescriptorHerpManager.h"
#include "../../../Engine/3D/ModelDraw.h"

#include <numbers>
#include <cmath>

uint32_t IEnemyEmitter::sSerialNumber_ = 0;

void IEnemyEmitter::Initialize(Model* model)
{
	LargeNumberOfObjects::Initialize(model);

	worldTransform_.Initialize();

	// 型番
	serialNum_ = sSerialNumber_;
	// 全体番号
	sSerialNumber_++;
	name_ = "EnemyEmitter:" + std::to_string(serialNum_);
	// 初期化
	isRotateReturn_ = false;

}

void IEnemyEmitter::Update()
{
	if (isRotateReturn_ && !interval_.IsActive()) {
		isRotateReturn_ = false;
	}
	// リストの更新処理
	LargeNumberOfObjects::Update();

	interval_.Update();

	// フラグによる死亡処理
	objects_.remove_if([this](std::unique_ptr<OneOfManyObjects>& enemy) {
		if (enemy->IsDead()) {
			enemy.reset();
			return true;
		}
		return false;
		});

	// 回転処理
	nowAngle_ += 1.0f / rotation_;

	// 雑な一周リセット処理
	float oneLap = 6.28f;
	if (nowAngle_ >= oneLap) {
		nowAngle_ = 0;
		isRotateReturn_ = true;
		interval_.Start(10.0f);
	}

	worldTransform_.transform_.rotate.z = nowAngle_;
	worldTransform_.UpdateMatrix();
}

void IEnemyEmitter::InitializeEmitter(float spinSpeed)
{

	rotation_ = spinSpeed;

}

void IEnemyEmitter::CreateEnemy(const Vector3& transformPosition, float distance, uint32_t enemyCount)
{
	// エミッター側に書き込み
	worldTransform_.transform_.translate = transformPosition;
	distance_ = distance;
	maxCount_ = enemyCount;
	// 敵の角度生成
	float angleIncrement = 2.0f * (float)std::numbers::pi / maxCount_;
	Vector3 center = worldTransform_.GetWorldPosition();
	// 敵の生成
	for (uint32_t i = 0; i < maxCount_; ++i) {
		// 角度からオフセットの計算
		float angle = i * angleIncrement;
		Vector3 newPosition = {};
		newPosition.x = center.x + (std::cosf(angle) * distance_);
		newPosition.y = center.y + (std::sinf(angle) * distance_);

		// 生成
		std::unique_ptr<OneOfManyObjects> obj = std::make_unique<Enemy>();
		obj->Initialize();
		//static_cast<Enemy*>(obj.get())->SetParent(&worldTransform_);
		static_cast<Enemy*>(obj.get())->SetEmitter(this);
		static_cast<Enemy*>(obj.get())->SetDefaultOffset(newPosition);
		obj->transform_.translate = newPosition;
		// 初期化
		static_cast<Enemy*>(obj.get())->StateInitialize(std::make_unique<EnemyGroundState>(), 0);
		// リストに追加
		objects_.push_back(std::move(obj));
	}

}

void IEnemyEmitter::Draw(BaseCamera& camera, std::vector<UINT>* textureHnadles)
{
	Map(camera.GetViewProjectionMatrix());

	ModelDraw::ManyAnimObjectsDesc desc;
	desc.camera = &camera;
	desc.materialsHandle = &materialsHandleGPU_;
	desc.model = model_;
	desc.numInstance = numInstance_;
	if (textureHnadles) {
		desc.textureHandles = *textureHnadles;
	}
	desc.transformationMatrixesHandle = &transformationMatrixesHandleGPU_;
	//desc.localMatrixesHandle = &local;
	ModelDraw::ManyAnimObjectsDraw(desc);
}

void IEnemyEmitter::ImGuiDraw()
{
	ImGui::Text(name_.c_str());

	ImGui::Text("returnFlag : %d", this->isRotateReturn_);

	std::string fullPath = name_ + "Position";
	ImGui::DragFloat3(fullPath.c_str(), &worldTransform_.transform_.translate.x, 0.01f, -100, 100);
	fullPath = name_ + "Rotate";
	ImGui::DragFloat3(fullPath.c_str(), &worldTransform_.transform_.rotate.x, 0.01f, 0, 100);
	
	for (std::list<std::unique_ptr<OneOfManyObjects>>::iterator it = objects_.begin();
		it != objects_.end(); ++it) {
		static_cast<Enemy*>((*it).get())->ImGuiDraw();
	}


	ImGui::Text("\n");

}
