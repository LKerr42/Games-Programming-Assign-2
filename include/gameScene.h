#ifndef GAME_SCENE_H_
#define GAME_SCENE_H_

#include <engine.h>

#include <sceneManager.h>
#include <button.h>
#include <collisions.h>
#include <transform.h>
#include <hero.h>
#include <alien.h>
#include <sound.h>

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
        Vec2 pos;
        const Vec2 size = Vec2(40, 40);

        Wall(float x, float y) {pos = Vec2(x, y);};
    } Wall;

    typedef struct DisplayElement {
        char* dialogue;
        int fontSize;
        Texture* mainTexture;
    } DisplayElement;

    typedef struct upgradePurchase {
        int indx;
        int cost;
        float value;
        char* title;
    } upgradePurchase;

    enum DisplayState : int {
        HUD,
        ELEMENT,
        WORKBENCH
    };

    void init();
    void update(float dt);
    void render(float lag);
    void close();

    void displayDialogue(const char *msg);
    void setupShadows();
    void smoothShadows();
    void runButton(Button &b);
    bool attemptPurchase(upgradePurchase& purchase, float& data, float updateVal);

    void addAliensForWave(int numHatching, int numMature, int numSpitter, int waveNum);

    void handleCollision(Rect collidingWith);
    bool pointInTriangle(Vec2 p, Vec2 a, Vec2 b, Vec2 c);

    void deleteLazer(Laser& L, int indx);
}

#endif