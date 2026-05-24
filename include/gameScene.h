#ifndef GAME_SCENE_H_
#define GAME_SCENE_H_

#include <engine.h>

#include <sceneManager.h>
#include <button.h>
#include <collisions.h>
#include <transform.h>
#include <hero.h>

#include <iostream>
#include <vector>
#include <list>

// GameScene Namespace
namespace GameScene {
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

    typedef struct DisplayElement {
        char* dialogue;
        int fontSize;
        Texture* mainTexture;
    } DisplayElement;

    enum Level {
        LEVEL_ONE,
        LEVEL_TWO,
        LEVEL_THREE
    };

    enum DisplayState {
        HUD,
        ELEMENT
    };

    void init();
    void update(float dt);
    void render(float lag);
    void close();

    void openItemImages();
    void displayDialogue(const char *msg);
}

#endif