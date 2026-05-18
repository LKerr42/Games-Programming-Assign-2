#ifndef ITEMS_H_
#define ITEMS_H_

#include <engine.h>

enum UpgradeType {
    DASH,
    MELEE,
    INVISIBILITY
};

typedef struct Weapon {
    float fireSpeed, reloadSpeed, energy;
    Texture largeTex, smallTex;
} Weapon;

typedef struct Armour {
    float resistance, healthUpgrade;
    Texture largeTex, smallTex;
} Armour;

typedef struct Upgrade {
    UpgradeType type;
    float value;
    Texture largeTex, smallTex;
} Upgrade;

#endif