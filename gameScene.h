#ifndef GAME_SCENE_H_
#define GAME_SCENE_H_

#include <engine.h>

//#include <button.h>
#include <sceneManager.h>

// GameScene Namespace, declaring all the basic functions to run the game
namespace GameScene {
    void init();
    void update(float dt);
    void render(float lag);
    void close();
}

#endif