#include <hero.h>

Hero::Hero() {
    transform = Transform(Vec2(150, WINDOW_HEIGHT/2.0f), 0.0f, Vec2(64, 40));
    health = 100;
    sightRad = 70.0f;
    sightLength = 200.0f;
    detectionInner = 150.0f;
    detectionOuter = 300.0f;

    currWeapon = nullptr; 
    currArmour = nullptr; 
    currUpgrade = nullptr;
};