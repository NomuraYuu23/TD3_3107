#include "Sprite.h"
#include "../base/TextureManager.h"
#include "../base/WinApp.h"
#include <cassert>
#include "../base/BufferResource.h"

using namespace DirectX;
using namespace Microsoft::WRL;

// デバイス
ID3D12Device* Sprite::sDevice = nullptr;
// ディスクリプタサイズ
UINT Sprite::sDescriptorHandleIncrementSize;
// コマンドリスト
ID3D12GraphicsCommandList* Sprite::sCommandList = nullptr;
// ルートシグネチャ
ID3D12RootSignature* Sprite::sRootSignature;
// パイプラインステートオブジェクト
ID3D12PipelineState* Sprite::sPipelineState;

/// <summary>
/// 静的初期化
/// </summary>
/// <param name="device">デバイス</param>
void Sprite::StaticInitialize(
	ID3D12Device* device,
	ID3D12RootSignature* rootSignature,
	ID3D12PipelineState* pipelineState) {

	assert(device);
	
	sDevice = device;

	sDescriptorHandleIncrementSize = sDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	sRootSignature = rootSignature;
	sPipelineState = pipelineState;

}

/// <summary>
/// 静的前処理
/// </summary>
/// <param name="cmdList">描画コマンドリスト</param>
void Sprite::PreDraw(ID3D12GraphicsCommandList* cmdList) {

	assert(Sprite::sCommandList == nullptr);

	sCommandList = cmdList;

	//RootSignatureを設定。
	sCommandList->SetPipelineState(sPipelineState);//PS0を設定
	sCommandList->SetGraphicsRootSignature(sRootSignature);
	//形状を設定。PS0に設定しているものとは別。
	sCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

}

/// <summary>
/// 描画後処理
/// </summary>
void Sprite::PostDraw() {
	//コマンドリストを解除
	Sprite::sCommandList = nullptr;

}

/// <summary>
/// スプライト生成
/// </summary>
/// <param name="textureHandle">テクスチャハンドル</param>
/// <param name="position">座標</param>
/// <param name="color">色</param>
/// <param name="anchorpoint">アンカーポイント</param>
/// <param name="isFlipX">左右反転</param>
/// <param name="isFlipY">上下反転</param>
/// <returns>生成されたスプライト</returns>
Sprite* Sprite::Create(
	uint32_t textureHandle, const Vector2& position, const Vector4& color) {

	// 仮サイズ
	Vector2 size = { 100.0f, 100.0f };

	// テクスチャ情報取得
	const D3D12_RESOURCE_DESC& resDesc = TextureManager::GetInstance()->GetResourceDesc(textureHandle);
	// スプライトのサイズをテクスチャのサイズに設定
	size = { (float)resDesc.Width, (float)resDesc.Height };

	// Spriteのインスタンスを生成
	Sprite* sprite = new Sprite(textureHandle, position, size, color);
	if (sprite == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!sprite->Initialize()) {
		delete sprite;
		assert(0);
		return nullptr;
	}

	return sprite;

}

/// <summary>
/// コンストラクタ
/// </summary>
Sprite::Sprite() {}

/// <summary>
/// コンストラクタ
/// </summary>
Sprite::Sprite(
	uint32_t textureHandle, const Vector2& position, const Vector2& size, const Vector4& color) {

	textureHandle_ = textureHandle;

	//WVP用のリソースを作る。Matrix4x4 1つ分のサイズを用意する
	spriteForGPUBuff_ = BufferResource::CreateBufferResource(DirectXCommon::GetInstance()->GetDevice(), (sizeof(SpriteForGPU) + 0xff) & ~0xff);
	//書き込むためのアドレスを取得
	spriteForGPUBuff_->Map(0, nullptr, reinterpret_cast<void**>(&spriteForGPUMap_));

	spriteForGPUMap_->World = Matrix4x4::MakeIdentity4x4();
	spriteForGPUMap_->WVP = Matrix4x4::MakeIdentity4x4();
	transformMatrix_ = Matrix4x4::MakeIdentity4x4();

	// 位置
	SetPosition(position);
	// 回転
	SetRotate(rotate_);	
	//大きさ
	size_ = size;

	//アンカーポイント
	anchorPoint_ = { 0.5f, 0.5f };

	// フリップ
	isFlipX_ = false;
	isFlipY_ = false;

	// 非表示フラグ
	isInvisible_ = false;

	// テクスチャのサイズ
	textureSize_ = size;
	// 描画する
	textureLeftTop_ = {0.0f,0.0f};
	// テクスチャ初期サイズ
	textureInitSize_ = size;

	// マテリアル
	material_ = std::make_unique<Material>();
	material_->Initialize();

	uvTransform_ = { 1.0f, 1.0f, 1.0f,
					 0.0f, 0.0f, 0.0f,
					 0.0f, 0.0f, 0.0f };
	color_ = color;
	enableLighting_ = EnableLighting::None;

	material_->Update(uvTransform_, color_, enableLighting_, 0.0f);

}

/// <summary>
/// 初期化
/// </summary>
/// <returns>成否</returns>
bool Sprite::Initialize() {

	assert(sDevice);

	//HRESULT hr;

	resourceDesc_ = TextureManager::GetInstance()->GetResourceDesc(textureHandle_);

	//Sprite用の頂点リソースを作る
	vertBuff_ = BufferResource::CreateBufferResource(sDevice, sizeof(VertexData) * 6);

	//リソースの先頭のアドレスから使う
	vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点6つ分のサイズ
	vbView_.SizeInBytes = sizeof(VertexData) * 6;
	//1頂点あたりのサイズ
	vbView_.StrideInBytes = sizeof(VertexData);

	//書き込むためのアドレスを取得
	vertBuff_->Map(0, nullptr, reinterpret_cast<void**>(&vertMap));

	//インデックスリソースを作る
	indexBuff_ = BufferResource::CreateBufferResource(sDevice, sizeof(uint32_t) * 6);

	//インデックスバッファビュー
	//リソースの先頭のアドレスから使う
	ibView_.BufferLocation = indexBuff_->GetGPUVirtualAddress();
	//使用するリソースのサイズはインデックス6つ分のサイズ
	ibView_.SizeInBytes = sizeof(uint32_t) * 6;
	//インデックスはuint32_tとする
	ibView_.Format = DXGI_FORMAT_R32_UINT;

	//インデックスリソースにデータを書き込む
	indexBuff_->Map(0, nullptr, reinterpret_cast<void**>(&indexMap));

	SetAnchorPoint(anchorPoint_);

	return true;

}

/// <summary>
/// 更新
/// </summary>
void Sprite::Update() {

}

/// <summary>
/// テクスチャハンドルの設定
/// </summary>
/// <param name="textureHandle"></param>
void Sprite::SetTextureHandle(uint32_t textureHandle) {

	textureHandle_ = textureHandle;
	resourceDesc_ = TextureManager::GetInstance()->GetResourceDesc(textureHandle_);

}

/// <summary>
/// 描画
/// </summary>
void Sprite::Draw() {

	// 非表示
	if (isInvisible_) {
		return;
	}

	// 頂点バッファの設定
	sCommandList->IASetVertexBuffers(0, 1, &vbView_);
	//IBVを設定
	sCommandList->IASetIndexBuffer(&ibView_);
	
	//Sprite用のWorldViewProjectionMatrixを作る
	Matrix4x4 viewMatrixSprite = Matrix4x4::MakeIdentity4x4();
	Matrix4x4 projectionMatrixSprite = Matrix4x4::MakeOrthographicMatrix(0.0f, 0.0f, float(WinApp::kWindowWidth), float(WinApp::kWindowHeight), 0.0f, 100.0f);
	Matrix4x4 worldViewProjectionMatrixSprite = Matrix4x4::Multiply(transformMatrix_, Matrix4x4::Multiply(viewMatrixSprite, projectionMatrixSprite));
	spriteForGPUMap_->WVP = worldViewProjectionMatrixSprite;
	spriteForGPUMap_->World = transformMatrix_;

	//TransformationMatrixCBufferの場所を設定
	sCommandList->SetGraphicsRootConstantBufferView(1, spriteForGPUBuff_->GetGPUVirtualAddress());

	//マテリアルCBufferの場所を設定
	sCommandList->SetGraphicsRootConstantBufferView(0, material_->GetMaterialBuff()->GetGPUVirtualAddress());

	// シェーダーリソースビューをセット
	TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(sCommandList, 2, textureHandle_);

	//描画
	sCommandList->DrawIndexedInstanced(kVertNum, 1, 0, 0, 0);

}

void Sprite::SetTextureRange()
{

	uvTransform_.translate.x = textureLeftTop_.x / textureInitSize_.x;
	uvTransform_.translate.y = textureLeftTop_.y / textureInitSize_.y;

	uvTransform_.scale.x = textureSize_.x / textureInitSize_.x;
	uvTransform_.scale.y = textureSize_.y / textureInitSize_.y;

	material_->Update(uvTransform_, color_, enableLighting_, 0.0f);
}

void Sprite::TransformMatrixUpdate()
{

	//拡大縮小行列
	Matrix4x4 scaleMatrix = Matrix4x4::MakeScaleMatrix(Vector3{ 1.0f, 1.0f, 1.0f });
	// 回転行列

	Matrix4x4 rotateMatrix = Matrix4x4::MakeRotateZMatrix(rotate_);


	//平行移動行列
	Matrix4x4 translateMatrix = Matrix4x4::MakeTranslateMatrix(Vector3{ position_.x, position_.y, 0.0f });

	// 行列
	transformMatrix_ = Matrix4x4::Multiply(scaleMatrix, Matrix4x4::Multiply(rotateMatrix, translateMatrix));

}

void Sprite::SetPosition(const Vector2& position)
{

	position_ = position;

	TransformMatrixUpdate();

}

void Sprite::SetRotate(float rotate)
{

	rotate_ = rotate;

	TransformMatrixUpdate();

}

void Sprite::SetSize(const Vector2& size)
{

	size_.x = size.x;
	size_.y = size.y;

	SetAnchorPoint(anchorPoint_);

}

void Sprite::SetAnchorPoint(const Vector2& anchorPoint)
{
	anchorPoint_ = anchorPoint;

	// 頂点データ
	float left = (0.0f - anchorPoint_.x) * size_.x;
	float right = (1.0f - anchorPoint_.x) * size_.x;
	float top = (0.0f - anchorPoint_.y) * size_.y;
	float bottom = (1.0f - anchorPoint_.y) * size_.y;

	// 反転するか
	if (isFlipX_) {
		left = -left;
		right = -right;
	}
	if (isFlipY_) {
		top = -top;
		bottom = -bottom;
	}

	//一枚目の三角形
	vertMap[0].position = { left, bottom, 0.0f, 1.0f };//左下
	vertMap[0].texcoord = { 0.0f, 1.0f };
	vertMap[0].normal = { 0.0f, 0.0f, -1.0f };
	vertMap[1].position = { left, top, 0.0f, 1.0f };//左上
	vertMap[1].texcoord = { 0.0f, 0.0f };
	vertMap[1].normal = { 0.0f, 0.0f, -1.0f };
	vertMap[2].position = { right, bottom, 0.0f, 1.0f };//右下
	vertMap[2].texcoord = { 1.0f, 1.0f };
	vertMap[2].normal = { 0.0f, 0.0f, -1.0f };
	//ニ枚目の三角形
	vertMap[3].position = { right, top, 0.0f, 1.0f };//右上
	vertMap[3].texcoord = { 1.0f, 0.0f };
	vertMap[3].normal = { 0.0f, 0.0f, -1.0f };
	vertMap[4].position = { left, top, 0.0f, 1.0f };//左上
	vertMap[4].texcoord = { 0.0f, 0.0f };
	vertMap[4].normal = { 0.0f, 0.0f, -1.0f };
	vertMap[5].position = { right, bottom, 0.0f, 1.0f };//右下
	vertMap[5].texcoord = { 1.0f, 1.0f };
	vertMap[5].normal = { 0.0f, 0.0f, -1.0f };

	//インデックスリソースにデータを書き込む
	indexMap[0] = 0;
	indexMap[1] = 1;
	indexMap[2] = 2;
	indexMap[3] = 1;
	indexMap[4] = 3;
	indexMap[5] = 2;

}

void Sprite::SetIsFlipX(bool isFlipX)
{
	isFlipX_ = isFlipX;
	SetAnchorPoint(anchorPoint_);

}

void Sprite::SetIsFlipY(bool isFlipY)
{
	isFlipY_ = isFlipY;
	SetAnchorPoint(anchorPoint_);
}

void Sprite::SetTextureSize(const Vector2& textureSize)
{
	textureSize_ = textureSize;
	SetTextureRange();
}
void Sprite::SetTextureLeftTop(const Vector2& textureLeftTop)
{
	textureLeftTop_ = textureLeftTop;
	SetTextureRange();
}

void Sprite::SetUvTransform(const TransformStructure& uvTransform)
{

	uvTransform_ = uvTransform;

	material_->Update(uvTransform_, color_, enableLighting_, 0.0f);

}

void Sprite::SetUvTransform(const Vector3& scale, const Vector3& rotate, const Vector3& translate)
{

	uvTransform_.scale = scale;
	uvTransform_.rotate = rotate;
	uvTransform_.translate = translate;

	material_->Update(uvTransform_, color_, enableLighting_, 0.0f);

}

void Sprite::SetColor(const Vector4& color)
{

	color_ = color;

	material_->Update(uvTransform_, color_, enableLighting_, 0.0f);

}

void Sprite::SetEnableLighting(int enableLighting)
{

	enableLighting_ = enableLighting;

	material_->Update(uvTransform_, color_, enableLighting_, 0.0f);

}
