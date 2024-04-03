#pragma once
#include <cstdInt>

//プレイヤー
inline static constexpr uint32_t kCollisionAttributePlayer = 0b1;
// 武器
inline static constexpr uint32_t kCollisionAttributeWeapon = 0b1 << 1;
// 敵
inline static constexpr uint32_t kCollisionAttributeEnemy = 0b1 << 2;
// 地形ブロック
inline static constexpr uint32_t kCollisionAttributeTerrain = 0b1 << 3;
