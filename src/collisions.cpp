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

// Collision (Rect-Rect)
bool collision(Rect rect1, float rectAngle1, Rect rect2, float rectAngle2) {
    return collision(Vec2(rect1.x, rect1.y), Vec2(rect1.width, rect1.height), rectAngle1,
                    Vec2(rect2.x, rect2.y), Vec2(rect2.width, rect2.height), rectAngle2);
}

// Collision (Transform-Transform)
bool collision(Transform transform1, Transform transform2) {
    return collision(transform1.getPosition(), transform1.getSize(), transform1.getAngle(),
                    transform2.getPosition(), transform2.getSize(), transform2.getAngle());
}

// Collision (AABB-Circle)
bool collision(Vec2 pos1, float radius1, Vec2 pos2, Vec2 size2) {
    float tx, ty;

    if (collision(pos1, pos2, size2)) {
        return true;
    }

    // Find point on Rectangle closest to circle x
    if (pos1.x < pos2.x) { //left
        tx = pos2.x; 
    } else if (pos1.x > pos2.x + size2.x) { //right
        tx = pos2.x + size2.x; 
    } else { //circle
        tx = pos1.x; 
    }

    // Find point on Rectangle closest to Circle y
    if (pos1.y < pos2.y) { //top
        ty = pos2.y; 
    } else if (pos1.y > pos2.y + size2.y) { //bottom
        ty = pos2.y + size2.y; 
    } else { // circle
        ty = pos1.y; 
    }

    // Compare distance between circle centre and closest point
    if (distance(Vec2(tx, ty), pos1) < radius1) {
        return true;
    }
    return false;
}