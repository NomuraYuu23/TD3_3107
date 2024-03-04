#pragma once
#include <cstdInt>

//プレイヤー
const uint32_t kCollisionAttributePlayer = 0b1;
//ブロック
const uint32_t kCollisionAttributeBlock = 0b1 << 1;
// 武器
const uint32_t kCollisionAttributeWeapon = 0b1 << 2;
// 敵
const uint32_t kCollisionAttributeEnemy = 0b1 << 3;

