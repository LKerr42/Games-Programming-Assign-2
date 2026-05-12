#ifndef GAME_SCENE_H_
#define GAME_SCENE_H_

#include <engine.h>

#include <button.h>
#include <sceneManager.h>

#include <cfloat>
#include <vector>

// GameScene Namespace
namespace GameScene {
    class Hero {
        public:
            Vec2 position;
            Vec2 size;
            float angle;
            int health;
            Texture tex;
            Rect boundingBox;

            Vec2 getBBpos() { return Vec2(boundingBox.x, boundingBox.y); }
            Vec2 getBBsize() { return Vec2(boundingBox.width, boundingBox.height); }

            Hero();
    };

    class Laser {
        public:
            Vec2 position;
            Vec2 size;
            float speed;
            float angle;

            Laser();
    };

    void init();
    void update(float dt);
    void render(float lag);
    void close();
}

#endif