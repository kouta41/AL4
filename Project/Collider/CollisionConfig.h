/// <summary>
/// コリジョンコンフィグ
/// </summary>
#pragma once

#include <cstdint>


const uint32_t kAttributeEnemy = 0b0001;
const uint32_t kAttributeBlock = 0b0010;
const uint32_t kAttributePlayer = 0b0100;


//形状
const uint32_t kCollisionSphere = 0b1;
const uint32_t kCollisionAABB = 0b1 << 1;

