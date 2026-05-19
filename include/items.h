#ifndef ITEMS_H_
#define ITEMS_H_

#include <engine.h>

enum UpgradeType {
    DASH,
    MELEE,
    INVISIBILITY
};

class Item {
    public:
        Texture *largeTexture, *smallTexture;
        Rect dst;

        Item(Texture *large, Texture *small, Rect d) : largeTexture(large), smallTexture(small), dst(d) {
            dst.width = dst.height = 40;
        };

        virtual ~Item() = default;
};

class Weapon : public Item {
    public:
        float fireSpeed, reloadSpeed;
        float currEnergy, fullEnergy;

        Weapon(Texture *large, Texture *small, Rect d, float fs, float rs,float fe) : Item(large, small, d) {
            fireSpeed = fs;
            reloadSpeed = rs;
            currEnergy = fe;
            fullEnergy = fe;
        };
};

class Armour : public Item {
    public:
        float resistance, healthUpgrade;

        Armour(Texture *large, Texture *small, Rect d, float r, float hu) : Item(large, small, d){
            resistance = r;
            healthUpgrade = hu;
        };
};

class Upgrade : public Item {
    public:
        UpgradeType type;
        float value;

        Upgrade(Texture *large, Texture *small, Rect d, UpgradeType t, float v) : Item(large, small, d) {
            type = t;
            value = v;
        };
};

#endif