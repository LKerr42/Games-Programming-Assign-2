#ifndef SCENE_MANAGER_H_
#define SCENE_MANAGER_H_

// scenes enumeration
enum Scene {
    MENU, 
    GAME
};

// Load all of the data and initalise as needed for a scene
void loadScene(Scene scene);

#endif