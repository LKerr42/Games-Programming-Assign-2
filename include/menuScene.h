#ifndef MENU_SCENE_H_
#define MENU_SCENE_H_

#include <vector>

#include <engine.h>
#include <text.h>

#include <button.h>
#include <collisions.h>
#include <sceneManager.h>

// MenuScene Namespace, declaring all the basic functions to run the menu
namespace MenuScene {
    void init();
    void update(float dt);
    void render(float lag);
    void close();
}

#endif