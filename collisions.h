#ifndef COLLISION_H_
#define COLLISION_H_

#include <engine.h>
#include <transform.h>
#include <algorithm>

// Collision (Point-AABB)
bool collision(Vec2 pos1, Vec2 pos2, Vec2 size2);

// Collision (AABB-AABB)
bool collision(Vec2 pos1, Vec2 size1, Vec2 pos2, Vec2 size2);

// Collision (Point-Rectangle)
bool collision(Vec2 pos1, Vec2 pos2, Vec2 size2, float angle2);

// Collision (Rectangle-Rectangle)
bool collision(Vec2 pos1, Vec2 size1, float angle1, Vec2 pos2, Vec2 size2, float angle2);

// Collision (Rect-Rectangle)
bool collision(Rect rect, float rectAngle, Vec2 pos1, Vec2 size1, float angle1);

// Collision (Transform-Transform)
bool collision(Transform transform1, Transform transform2);

// Collision (AABB-Rectangle)
// (Not to be used directly)
bool collisionAABBRectangle(Vec2 pos0, Vec2 size0, Vec2 pos1, Vec2 size1, float angle1);

#endif