#include <collisions.h>

//Point-AABB
bool collision(Vec2 pos1, Vec2 pos2, Vec2 size) {
    return (pos1.x < pos2.x + size.x) &&
            (pos2.x < pos1.x) &&
            (pos1.y < pos2.y + size.y) &&
            (pos2.y < pos1.y);
}

//AABB-AABB
bool collision(Vec2 pos1, Vec2 size1, Vec2 pos2, Vec2 size2) {
    return (pos1.x < pos2.x + size2.x) &&
           (pos2.x < pos1.x + size1.x) &&
           (pos1.y < pos2.y + size2.y) &&
           (pos2.y < pos1.y + size1.y);
}

//AABB-Rectangle (Do not use directly)
bool collisionAABBRectangle(Vec2 pos1, Vec2 size1, Vec2 pos2, Vec2 size2, float angle1) {
    Vec2 centre1 = pos2 + size2/2;

    Vec2 a = centre1 + rotate(pos2 - centre1, angle1);
    Vec2 b = centre1 + rotate(pos2 + Vec2(size2.x, 0) - centre1, angle1);
    Vec2 c = centre1 + rotate(pos2 + Vec2(0, size2.y) - centre1, angle1);
    Vec2 d = centre1 + rotate(pos2 + size2 - centre1, angle1);

    float left = std::min({a.x,b.x, c.x,d.x});
    float right = std::max({a.x,b.x,c.x,d.x});
    float top = std::min({a.y,b.y,c.y,d.y});
    float bottom = std::max({a.y,b.y,c.y,d.y});

    return collision(pos1, size1, Vec2(left, top), Vec2(right-left, bottom-top));
}

//Point-Rectangle
bool collision(Vec2 pos1, Vec2 pos2, Vec2 size, float angle) {
    Vec2 centre1 = pos2 + size/2;

    Vec2 pos1_r = centre1 + rotate(pos1 - centre1, - angle);

    return collision(pos1_r, pos2, size);
}

//Rectangle-Rectangle
bool collision(Vec2 pos1, Vec2 size1, float angle1, Vec2 pos2, Vec2 size2, float angle2) {
    Vec2 centre1 = pos1 + size1/2;
    Vec2 centre2 = pos2 + size2/2;

    Vec2 pos1_r = centre2 + rotate(centre1 - centre2, - angle2) - size1/2;
    Vec2 pos2_r = centre1 + rotate(centre2 - centre1, - angle1) - size2/2;

    return collisionAABBRectangle(pos1, size1, pos2_r, size2, angle2-angle1) &&
           collisionAABBRectangle(pos2, size2, pos1_r, size1, angle1-angle2);
}

// Collision (Rect-Rectangle)
bool collision(Rect rect, float rectAngle, Vec2 pos1, Vec2 size1, float angle1) {
    return collision(Vec2(rect.x, rect.y), Vec2(rect.width, rect.height), rectAngle,
                    pos1, size1, angle1);
}

// Collision (Transform-Transform)
bool collision(Transform transform1, Transform transform2) {
    return collision(transform1.getPosition(), transform1.getSize(), transform1.getAngle(),
                    transform2.getPosition(), transform2.getSize(), transform2.getAngle());
}