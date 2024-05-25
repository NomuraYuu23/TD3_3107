#pragma once
#include <cstdint>
#include "../../../Engine/Math/Vector2.h"
#include "../3D/TransformStructure.h"

// エミッタ引数
struct EmitterDesc
{
    
    EulerTransform* transform; // トランスフォーム（参照）
    uint32_t instanceCount; // 一度に出すインスタンス数
    Vector3 velocity;       // ベクトル
    float frequency; // 発生頻度(秒)
    float lifeTime; // 生存時間
    uint32_t particleModelNum; // パーティクルのモデル
    uint32_t paeticleName; // パーティクルの名前

};
