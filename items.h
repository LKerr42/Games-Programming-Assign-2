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
        Texture *tex;
        Rect dst;

        Item(Texture *T, Rect d) : tex(T), dst(d) {
                dst.width = dst.height = 30;
            };

        virtual ~Item() = default;
};

class Weapon : public Item {
    public:
        float fireSpeed, reloadSpeed;
        float currEnergy, fullEnergy;

        Weapon(Texture *T, Rect d, float fs, float rs, float ce, float fe) : Item(T, d) {
            fireSpeed = fs;
            reloadSpeed = rs;
            currEnergy = ce;
            fullEnergy = fe;
        };
};

class Armour : public Item {
    public:
        float resistance, healthUpgrade;

        Armour(Texture *T, Rect d, float r, float hu) : Item(T, d){
            resistance = r;
            healthUpgrade = hu;
        };
};

class Upgrade : public Item {
    public:
        UpgradeType type;
        float value;

        Upgrade(Texture *T, Rect d, UpgradeType t, float v) : Item(T, d) {
            type = t;
            value = v;
        };
};

#endif