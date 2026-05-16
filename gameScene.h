#ifndef GAME_SCENE_H_
#define GAME_SCENE_H_

#include <engine.h>

#include <sceneManager.h>
#include <button.h>
#include <collisions.h>
#include <transform.h>

#include <vector>

// GameScene Namespace
namespace GameScene {
    class Hero {
        public:
            Transform transform;
            int health;
            Texture tex;

            Hero();
    };

    class Laser {
        public:
            Transform transform;
            float speed;

            Laser();
    };

    typedef struct Wall {
        Rect rect;
        bool active;
        Texture activeTex, inactiveTex;
    } Wall;

    enum Level {
        LEVEL_ONE,
        LEVEL_TWO,
        LEVEL_THREE
    };

    void init();
    void update(float dt);
    void render(float lag);
    void close();
}

#endif