#include <transform.h>

Transform::Transform(Vec2 pos, float angle, Vec2 size) {
    this->pos = pos;
    this->angle = angle;
    this->size = size;
}

void Transform::updateBoundingBox() {
    //calculate bounding box
    float minX = FLT_MAX, maxX = -FLT_MAX, minY = FLT_MAX, maxY = -FLT_MAX;
    float angleRads = angle * M_PI / 180.0f;

    Vec2 cornerPos[4] = {
        Vec2(-size.x/2, size.y/2),
        Vec2(size.x/2, size.y/2),
        Vec2(size.x/2, -size.y/2),
        Vec2(-size.x/2, -size.y/2)
    };

    for (int i = 0; i < 4; i++) {
        Vec2 currentPos = cornerPos[i];
        cornerPos[i].x = (currentPos.x * cos(angleRads) - currentPos.y * sin(angleRads)) + pos.x;
        cornerPos[i].y = (currentPos.x * sin(angleRads) + currentPos.y * cos(angleRads)) + pos.y;

        minX = min(minX, cornerPos[i].x);
        minY = min(minY, cornerPos[i].y);
        maxX = max(maxX, cornerPos[i].x);
        maxY = max(maxY, cornerPos[i].y);
    }

    boundingBox = (Rect){
        minX, minY,
        maxX - minX,
        maxY - minY
    };
}

Vec2 Transform::getPosition(Space space) { 
    if (space == GLOBAL) {
        return pos - size / 2;
    } else {
        return pos;
    }
};

void Transform::translate(Vec2 t) {
    pos += t;
}

void Transform::translateByAngle(float speed) {
    float angleRad = angle * M_PI / 180.0f;

    pos.x += cos(angleRad) * speed;
    pos.y += sin(angleRad) * speed;
}

void Transform::rotate(float angle) {
    this->angle += angle;
}

void Transform::rotateTo(Vec2 target) {
    Vec2 forward = target - pos;
    angle = atan2(forward.y, forward.x) / M_PI * 180.0f;
}

void Transform::scale(Vec2 scale) {
    // Local scale
    size = Vec2(size.x * scale.x, size.y * scale.y);
}