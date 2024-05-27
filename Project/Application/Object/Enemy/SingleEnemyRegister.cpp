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
	if (textureHnadles) {
		desc.textureHandles = *textureHnadles;
	}
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
