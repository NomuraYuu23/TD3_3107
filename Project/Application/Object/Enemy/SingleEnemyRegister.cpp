#include "SingleEnemyRegister.h"
#include "../../../Engine/2D/ImguiManager.h"
#include "../../../Engine/base/SRVDescriptorHerpManager.h"
#include "../../../Engine/3D/ModelDraw.h"

uint32_t SingleEnemyRegister::sSerialNumber_ = 0;

void SingleEnemyRegister::Initialize(Model* model)
{
	LargeNumberOfObjects::Initialize(model);

	// 型番
	serialNum_ = sSerialNumber_;
	// 全体番号
	sSerialNumber_++;
	name_ = "EnemySingle:" + std::to_string(serialNum_);

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

void SingleEnemyRegister::Update()
{

	LargeNumberOfObjects::Update();

	// フラグによる死亡処理
	objects_.remove_if([this](std::unique_ptr<OneOfManyObjects>& enemy) {
		if (enemy->IsDead()) {
			enemy.reset();
			return true;
		}
		return false;
		});

	// デバッグ以外の場合行う
	#ifndef _DEBUG

	// アニメーションの更新
	localMatrixManager_->SetNodeLocalMatrix(anim_.AnimationUpdate());
	localMatrixManager_->Map();

	#endif // !_DEBUG

}

void SingleEnemyRegister::Draw(BaseCamera& camera, std::vector<UINT>* textureHnadles)
{
	Map(camera.GetViewProjectionMatrix());

	ModelDraw::ManyAnimObjectsDesc desc;
	desc.camera = &camera;
	desc.materialsHandle = &materialsHandleGPU_;
	desc.model = model_;
	desc.numInstance = numInstance_;
	//if (textureHnadles) {
	//	desc.textureHandles = *textureHnadles;
	//}
	textureHnadles;
	desc.textureHandles = *texture_;
	desc.transformationMatrixesHandle = &transformationMatrixesHandleGPU_;
	desc.localMatrixManager = localMatrixManager_.get();
	ModelDraw::ManyAnimObjectsDraw(desc);
}

void SingleEnemyRegister::ImGuiDraw()
{
	//ImGui::Begin(name_.c_str());
	ImGui::Text(name_.c_str());
	//ImGui::End();
}

void SingleEnemyRegister::Edit(const std::string& name, const SingleEnemyData& singleEnemyData, Player* player)
{

	bool isEdit = false;

	std::list<std::unique_ptr<OneOfManyObjects>>::iterator itr = objects_.begin();
	for (; itr != objects_.end(); ++itr) {
		OneOfManyObjects* obj = itr->get();

		// 名前が一緒だったら
		if (static_cast<Enemy*>(obj)->GetName() == name) {

			obj->transform_.translate = singleEnemyData.position;
			Vector3 end = Vector3::Add(singleEnemyData.position, singleEnemyData.endPosition);
			static_cast<SingleEnemyState*>(static_cast<Enemy*>(obj)->GetNowState())->SettingMoveInfo(end);

			isEdit = true;
			break;
		}

	}

	// 編集してないので追加
	if (!isEdit) {
		std::unique_ptr<OneOfManyObjects> obj = std::make_unique<Enemy>();
		static_cast<Enemy*>(obj.get())->Initialize(name);
		obj->transform_.translate = singleEnemyData.position;
		// 初期化
		static_cast<Enemy*>(obj.get())->StateInitialize(std::make_unique<SingleEnemyState>(), singleEnemyData.typeNum);
		// 単体のやつ専用
		Vector3 end = obj->GetWorldPosition() + singleEnemyData.endPosition;
		static_cast<SingleEnemyState*>(static_cast<Enemy*>(obj.get())->GetNowState())->SettingMoveInfo(end);
		static_cast<Enemy*>(obj.get())->SetPlayer(player);
		// 追加
		objects_.push_back(std::move(obj));
	}

}

void SingleEnemyRegister::Edit(const std::string& name, const ChaseEnemyData& chaseEnemyData, Player* player)
{

	bool isEdit = false;

	std::list<std::unique_ptr<OneOfManyObjects>>::iterator itr = objects_.begin();
	for (; itr != objects_.end(); ++itr) {
		OneOfManyObjects* obj = itr->get();

		// 名前が一緒だったら
		if (static_cast<Enemy*>(obj)->GetName() == name) {
			obj->transform_.translate = chaseEnemyData.position;
			obj->Update();
			// 索敵設定
			static_cast<ChaseEnemyState*>(static_cast<Enemy*>(obj)->GetNowState())->SetChaseSetting(chaseEnemyData.searchLength);

			isEdit = true;
			break;
		}

	}

	// 編集してないので追加
	if (!isEdit) {
		std::unique_ptr<OneOfManyObjects> obj = std::make_unique<Enemy>();
		static_cast<Enemy*>(obj.get())->Initialize(name);
		obj->transform_.translate = chaseEnemyData.position;
		obj->Update();
		// 初期化
		static_cast<Enemy*>(obj.get())->StateInitialize(std::make_unique<ChaseEnemyState>(), 0);
		// プレイヤーセット
		static_cast<Enemy*>(obj.get())->SetPlayer(player);
		// 索敵設定
		static_cast<ChaseEnemyState*>(static_cast<Enemy*>(obj.get())->GetNowState())->SetChaseSetting(chaseEnemyData.searchLength);
		// 追加
		objects_.push_back(std::move(obj));
	}

}
