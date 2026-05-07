#include <engine.h>
#include <iostream>

#include <sceneManager.h>
#include <menuScene.h>
#include <gameScene.h>

Scene currentScene;

void loadScene(Scene scene) {
    // Close current Scence
    if (currentScene == MENU) {
        MenuScene::close();
    } else if (currentScene == GAME) {
        GameScene::close();
    }

    // Initialise new Scene
    if (scene == MENU) {
        MenuScene::init();
    } else if (scene == GAME) {
        GameScene::init();
    }

    // Set current Scene
    currentScene = scene;
}

// Initialise (called once at start)
void init() {
    // Example
    setWindowTitle("Pest Control - Menu");
    currentScene = MENU;

    MenuScene::init();
}

// Update Game
void update(float dt) {
    if (currentScene == MENU) {
        MenuScene::update(dt);
    } else if (currentScene == GAME) {
        GameScene::update(dt);
    }
}

// Render Game
void render(float lag) {
    if (currentScene == MENU) {
        MenuScene::render(lag);
    } else if (currentScene == GAME) {
        GameScene::render(lag);
    }
}

// Close the Game
void close() {
    if (currentScene == MENU) {
        MenuScene::close();
    } else if (currentScene == GAME) {
        GameScene::close();
    }
}