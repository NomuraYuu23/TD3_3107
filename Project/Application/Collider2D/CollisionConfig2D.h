#pragma once
#include <cstdInt>

//プレイヤー
const uint32_t kCollisionAttributePlayer = 0b1;
// 武器
const uint32_t kCollisionAttributeWeapon = 0b1 << 1;
// 敵
const uint32_t kCollisionAttributeEnemy = 0b1 << 2;
// 地形ブロック
const uint32_t kCollisionAttributeTerrain = 0b1 << 3;
