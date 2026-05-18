#ifndef HERO_H_
#define HERO_H_

#include <engine.h>

#include <transform.h>
#include <items.h>
    
class Hero {
    public:
        Transform transform;
        int health;
        Texture tex;

        Weapon currWeapon;
        Armour currArmour;
        Upgrade currUpgrade;

        Hero();
};

#endif