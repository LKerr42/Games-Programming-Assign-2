#ifndef COLLISION_H_
#define COLLISION_H_

#include <engine.h>
#include <algorithm>

// Collision (Point-AABB)
bool collision(Vec2 pos0, Vec2 pos1, Vec2 size1);

// Collision (AABB-AABB)
bool collision(Vec2 pos0, Vec2 size0, Vec2 pos1, Vec2 size1);

// Collision (Point-Rectangle)
bool collision(Vec2 pos0, Vec2 pos1, Vec2 size1, float angle1);

// Collision (Rectangle-Rectangle)
bool collision(Vec2 pos0, Vec2 size0, float angle0, Vec2 pos1, Vec2 size1, float angle1);

// Collision (AABB-Rectangle)
// (Not to be used directly)
bool collisionAABBRectangle(Vec2 pos0, Vec2 size0, Vec2 pos1, Vec2 size1, float angle1);

#endif