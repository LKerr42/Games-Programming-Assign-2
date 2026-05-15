#ifndef TRANSFORM_H_
#define TRANSFORM_H_

#include <engine.h>

#include <cfloat>

enum Space {
    GLOBAL, LOCAL
};

class Transform {
    private:
        Vec2 pos;
        Vec2 prevPos;
        Rect boundingBox;
        float angle;
        Vec2 size;
    public:
        Transform() {};
        Transform(Vec2 pos, float angle, Vec2 size);

        Vec2 getPosition(Space space = GLOBAL);
        float getAngle() { return angle; };
        Vec2 getSize() { return size; };

        void updateBoundingBox();
        Rect getBoundingBox() { return boundingBox; }

        void savePrevPos() { prevPos = pos; };
        void resetPos() { pos = prevPos; };

        void translate(Vec2 t);
        void translateByAngle(float speed);
        void rotate(float angle);
        void rotateTo(Vec2 target);
        void scale(Vec2 s);
};

#endif