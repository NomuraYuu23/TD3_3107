#include "IEnemyEmitter.h"
#include "Enemy.h"
#include "../../../Engine/2D/ImguiManager.h"
#include "../../../Engine/base/SRVDescriptorHerpManager.h"
#include "../../../Engine/3D/ModelDraw.h"
#include "../GameSystem/GameSystemManager.h"

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

	// デバッグ以外の場合行う
	#ifndef _DEBUG
	// アニメーション取得と初期化
	anim_.Initialize(
		model_->GetNodeAnimationData(),
		localMatrixManager_->GetInitTransform(),
		localMatrixManager_->GetNodeNames());

	// アニメーション開始
	anim_.StartAnimation(0, true);

	// アニメーションの更新
	localMatrixManager_->SetNodeLocalMatrix(anim_.AnimationUpdate());
	localMatrixManager_->Map();
	#endif // !
}

void IEnemyEmitter::Initialize(Model* model,const std::string& name)
{

	Initialize(model);
	name_ = name;

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
	#ifndef _DEBUG
	objects_.remove_if([this](std::unique_ptr<OneOfManyObjects>& enemy) {
		if (enemy->IsDead()) {
			enemy.reset();
			return true;
		}
		return false;
	});
	#endif // !_DEBUG

	// 回転処理
	nowAngle_ += 1.0f / (rotation_ * GameSystemManager::sGameSpeed);

	// 雑な一周リセット処理
	//float oneLap = 6.28f;
	if (isMinusRotation_) {
		if (nowAngle_ <= oneLapAngle_) {
			nowAngle_ = 0;
			isRotateReturn_ = true;
			interval_.Start(10.0f);
		}
	}
	else {
		if (nowAngle_ >= oneLapAngle_) {
			nowAngle_ = 0;
			isRotateReturn_ = true;
			interval_.Start(10.0f);
		}
	}
	// デバッグ以外の場合行う
	#ifndef _DEBUG

	// アニメーションの更新
	localMatrixManager_->SetNodeLocalMatrix(anim_.AnimationUpdate());
	localMatrixManager_->Map();

	#endif // !_DEBUG

	worldTransform_.transform_.rotate.z = nowAngle_;
	worldTransform_.UpdateMatrix();
}

void IEnemyEmitter::InitializeEmitter(float spinSpeed)
{

	rotation_ = spinSpeed;

	if (rotation_ > 0) {
		oneLapAngle_ = 6.28f;
		isMinusRotation_ = false;
	}
	else {
		oneLapAngle_ = -6.28f;
		isMinusRotation_ = true;
	}

}

void IEnemyEmitter::CreateEnemy(const Vector3& transformPosition, float distance, uint32_t enemyCount)
{
	// エミッター側に書き込み
	worldTransform_.transform_.translate = transformPosition;
	distance_ = distance;
	maxCount_ = enemyCount;
	// 敵の角度生成
	float angleIncrement = 0.0f;
	if (maxCount_ != 0) {
		angleIncrement = 2.0f * (float)std::numbers::pi / maxCount_;
	}

	float tAngle = 0;
	float add = 0;

	if (maxCount_ == 5) {
		if (rotation_ > 0) {
			tAngle = 0;
			add = 0.25f;
		}
		else if (rotation_ < 0) {
			tAngle = 0;
			add = -0.5f;
		}
	}

	GenerateEnemys(angleIncrement, tAngle, add);

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
	desc.localMatrixManager = localMatrixManager_.get();
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
	fullPath = name_ + "NowAngle";
	ImGui::DragFloat(fullPath.c_str(), &nowAngle_, 0.01f);
	
	fullPath = name_ + "rotation";
	ImGui::DragFloat(fullPath.c_str(), &rotation_, 0.01f);

	for (std::list<std::unique_ptr<OneOfManyObjects>>::iterator it = objects_.begin();
		it != objects_.end(); ++it) {
		static_cast<Enemy*>((*it).get())->ImGuiDraw();
	}


	ImGui::Text("\n");

}

void IEnemyEmitter::Edit(const MultiEnemyData& multiEnemyData)
{

	uint32_t count = 0;
	uint32_t preMaxCount = maxCount_;

	worldTransform_.transform_.translate = multiEnemyData.position;
	distance_ = multiEnemyData.distance;
	maxCount_ = multiEnemyData.enemyMaxCount;
	rotation_ = multiEnemyData.rotateSpeed;

	// 敵の角度生成
	float angleIncrement = 0.0f;
	if (maxCount_ != 0) {
		angleIncrement = 2.0f * (float)std::numbers::pi / maxCount_;
	}

	// オブジェクト分回す
	for (std::list<std::unique_ptr<OneOfManyObjects>>::iterator it = objects_.begin();
		it != objects_.end(); ++it) {

		// オブジェクトの数が最大より大きいなら削除
		if (maxCount_ <= count) {
			break;
		}

		// 距離、回転とか修正
		
		// 角度からオフセットの計算
		float angle = count * angleIncrement + (float)std::numbers::pi / 2.0f;
		Vector3 newPosition = {};
		newPosition.x = (std::cosf(angle) * distance_);
		newPosition.y = (std::sinf(angle) * distance_);
		static_cast<Enemy*>(it->get())->SetDefaultOffset(newPosition);
		it->get()->transform_.translate = newPosition;
		it->get()->Update();
		// カウントアップ
		count++;
	}

	// オブジェクトの数が最大より大きいなら削除
	if (preMaxCount > count) {

		objects_.erase(std::next(objects_.begin(), count), objects_.end());

	}
	// オブジェクトの数が足りてないなら生成
	else if(maxCount_ > count){

		while (true)
		{

			// オブジェクトの数が最大より大きいなら削除
			if (maxCount_ <= count) {
				break;
			}

			// 角度からオフセットの計算
			float angle = count * angleIncrement + (float)std::numbers::pi / 2.0f;
			Vector3 newPosition = {};
			newPosition.x = (std::cosf(angle) * distance_);
			newPosition.y = (std::sinf(angle) * distance_);

			// 生成
			std::unique_ptr<OneOfManyObjects> obj = std::make_unique<Enemy>();
			obj->Initialize();
			//static_cast<Enemy*>(obj.get())->SetParent(&worldTransform_);
			static_cast<Enemy*>(obj.get())->SetEmitter(this);
			static_cast<Enemy*>(obj.get())->SetDefaultOffset(newPosition);
			obj->transform_.translate = newPosition;
			obj->Update();
			// 初期化
			static_cast<Enemy*>(obj.get())->StateInitialize(std::make_unique<GroupEnemyState>(), 0);
			// リストに追加
			objects_.push_back(std::move(obj));

			count++;

		}

	}

}

void IEnemyEmitter::GenerateEnemys(float positionAngle, float transformAngle, float addAngle)
{
	// 敵の生成
	for (uint32_t i = 0; i < maxCount_; ++i) {
		// 角度からオフセットの計算
		float angle = i * positionAngle + (float)std::numbers::pi / 2.0f;
		Vector3 newPosition = {};
		newPosition.x = (std::cosf(angle) * distance_);
		newPosition.y = (std::sinf(angle) * distance_);

		// 生成
		std::unique_ptr<OneOfManyObjects> obj = std::make_unique<Enemy>();
		obj->Initialize();
		//static_cast<Enemy*>(obj.get())->SetParent(&worldTransform_);
		static_cast<Enemy*>(obj.get())->SetEmitter(this);
		static_cast<Enemy*>(obj.get())->SetDefaultOffset(newPosition);
		obj->transform_.translate = newPosition;
		obj->transform_.rotate.z = transformAngle;
		transformAngle += addAngle;
		obj->Update();
		// 初期化
		static_cast<Enemy*>(obj.get())->StateInitialize(std::make_unique<GroupEnemyState>(), 0);
		// リストに追加
		objects_.push_back(std::move(obj));
	}
}
