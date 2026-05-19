#include <hero.h>

Hero::Hero() {
    transform = Transform(Vec2(150, WINDOW_HEIGHT/2), 0.0f, Vec2(40, 25));
    health = 100;

    currWeapon = nullptr; 
    currArmour = nullptr; 
    currUpgrade = nullptr;
};