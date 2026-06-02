#ifndef HERO_H_
#define HERO_H_

#include <engine.h>

#include <transform.h>
#include <items.h>
    
class Hero {
    public:
        Texture tex;
        Transform transform;
        int health;
        float sightRad;
        float sightLength;
        float detectionOuter;
        float detectionInner;

        Weapon *currWeapon; 
        Armour *currArmour;
        Upgrade *currUpgrade;

        Hero();
};

#endif