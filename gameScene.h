#ifndef GAME_SCENE_H_
#define GAME_SCENE_H_

#include <engine.h>

#include <button.h>
#include <sceneManager.h>

// GameScene Namespace
namespace GameScene {
    class Hero {
        public:
            Vec2 position;
            Vec2 size;
            float angle;
            int health;
            Texture tex;

            Hero();
    };

    void init();
    void update(float dt);
    void render(float lag);
    void close();
}

#endif