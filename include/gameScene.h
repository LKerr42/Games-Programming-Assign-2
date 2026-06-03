#ifndef GAME_SCENE_H_
#define GAME_SCENE_H_

#include <engine.h>

#include <sceneManager.h>
#include <button.h>
#include <collisions.h>
#include <transform.h>
#include <hero.h>
#include <alien.h>

#include <iostream>
#include <vector>
#include <list>
#include <fstream>

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
    } Wall;

    typedef struct DisplayElement {
        char* dialogue;
        int fontSize;
        Texture* mainTexture;
    } DisplayElement;

    enum Level : int {
        LEVEL_ONE,
        LEVEL_TWO,
        LEVEL_THREE
    };

    enum DisplayState : int {
        HUD,
        ELEMENT
    };

    void init();
    void update(float dt);
    void render(float lag);
    void close();

    void displayDialogue(const char *msg);
    void setupShadows();
    void smoothShadows();
    void initWalls();

    void handleCollision(Rect collidingWith);

    bool pointInTriangle(Vec2 p, Vec2 a, Vec2 b, Vec2 c);

    void deleteLazer(Laser& L, int indx);
}

#endif