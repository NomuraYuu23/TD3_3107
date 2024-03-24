#pragma once
#include <d3d12.h>
#include <string>
#include <dxcapi.h>
#include <cassert>
#include <format>
#include <wrl.h>
#include <array>
#include <vector>

/// <summary>
/// ディスクリプタレンジの名前
/// </summary>
enum DescriptorRangeIndex {
	kDescriptorRangeIndexTexture0,
	kDescriptorRangeIndexTexture1,
	kDescriptorRangeIndexTexture2,
	kDescriptorRangeIndexTexture3,
	kDescriptorRangeIndexInstancing,
	kDescriptorRangeIndexPointLight,
	kDescriptorRangeIndexSpotLight,
	kDescriptorRangeIndexLocalMatrix,
	kDexcriptorRangeIndexCollider2DDebugDrawForGPU,
	kDescriptorRangeIndexOfCount,
};

/// <summary>
/// ルートパラメータの名前
/// </summary>
enum RootParameterIndex {
	kRootParameterIndexModel,
	kRootParameterIndexSprite,
	kRootParameterIndexParticle,
	kRootParameterIndexOutLine,
	kRootParameterIndexCollision2DDebugDraw,
	kRootParameterIndexLine,
	kRootParameterIndexSwapChain,
	kRootParameterIndexOfCount,
};

class RootParameterManager
{

public: // 関数

	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns></returns>
	static RootParameterManager* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	static void Initialize();

private: // 関数

	/// <summary>
	/// モデル
	/// </summary>
	static void RootParameterInitializeForModel();

	/// <summary>
	/// スプライト
	/// </summary>
	static void RootParameterInitializeForSprite();

	/// <summary>
	/// パーティクル
	/// </summary>	
	static void RootParameterInitializeForParticle();

	/// <summary>
	/// アウトライン
	/// </summary>	
	static void RootParameterInitializeForOutLine();

	/// <summary>
	/// コライダーデバッグ2D
	/// </summary>
	static void RootParameterInitializeForCollision2DDebugDraw();

	/// <summary>
	/// 線
	/// </summary>
	static void RootParameterInitializeForLine();

	/// <summary>
	/// ポストエフェクト
	/// </summary>
	static void RootParameterInitializeForSwapChain();

	/// <summary>
	/// ディスクリプタレンジの初期化
	/// </summary>
	static void DescriptorRangeInitialize();

public: // 変数

	// ルートパラメータ
	static std::array<std::vector<D3D12_ROOT_PARAMETER>, kRootParameterIndexOfCount> rootParameters_;
	// ディスクリプタレンジ
	static std::array<std::vector<D3D12_DESCRIPTOR_RANGE>, kDescriptorRangeIndexOfCount> descriptorRanges_;

private: // シングルトン

	RootParameterManager() = default;
	~RootParameterManager() = default;
	RootParameterManager(const RootParameterManager&) = delete;
	RootParameterManager& operator=(const RootParameterManager&) = delete;

};

