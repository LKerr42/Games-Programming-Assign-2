#ifndef ITEMS_H_
#define ITEMS_H_

#include <engine.h>

#include <animations.h>

class Hero;

enum UpgradeType {
    DASH,
    MELEE,
    INVISIBILITY,
    LONGER_SIGHT
};

class Item {
    public:
        Texture *smallTexture;
        Animation *animationLarge;
        Rect dst;

        Item(Animation *largeA, Texture *small, Rect d) : animationLarge(largeA), smallTexture(small), dst(d) {
            dst.width = dst.height = 40;
        };

        virtual void pickup(Hero& hero) = 0;
        virtual Item* dropItem(Hero& hero) = 0;
        virtual ~Item() = default;
};

class Weapon : public Item {
    public:
        float currEnergy, fullEnergy;
        Timer *fireTimer, *reloadTimer;

        Weapon(Animation *largeA, Texture *small, Rect d, float fs, float rs, float fe);

        void pickup(Hero& hero) override;
        Item* dropItem(Hero& hero) override;
};

class Armour : public Item {
    public:
        float resistance, healthUpgrade;

        Armour(Animation *largeA, Texture *small, Rect d, float r, float hu);

        void pickup(Hero& hero) override;
        Item* dropItem(Hero& hero) override;
};

class Upgrade : public Item {
    public:
        UpgradeType type;
        Timer *cooldown;
        float value;

        Upgrade(Animation *largeA, Texture *small, Rect d, UpgradeType t, float cds, float v);

        void pickup(Hero& hero) override;
        Item* dropItem(Hero& hero) override;
};

#endif