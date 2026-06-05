#ifndef ITEMS_H_
#define ITEMS_H_

#include <engine.h>

#include <animations.h>

#include <vector>
#include <list>

namespace GameScene {
    typedef struct DisplayElement DisplayElement;
    enum DisplayState : int;
}

class Hero;

enum UpgradeType {
    NONE,
    DASH,
    MELEE,
    INVISIBILITY,
    LONGER_SIGHT
};

enum ElementAction {
    SHOW_DISPLAY,
    OPEN_WORKBENCH,
    START_HORDE,
    HEAL_HERO
};

// -- polymorphic items -
class Item {
    public:
        Texture *smallTexture;

        Item(Texture *small) : smallTexture(small) {};

        virtual ~Item() = default;
};

class Weapon : public Item {
    public:
        float currEnergy, fullEnergy;
        Timer *fireTimer, *reloadTimer;

        Weapon(Texture *small, float fs, float rs, float fe);
};

class Armour : public Item {
    public:
        float resistance, healthUpgrade;

        Armour(Texture *small, float r, float hu);
};

class Upgrade : public Item {
    public:
        UpgradeType type;
        Timer *cooldown;
        float value;

        Upgrade(Texture *small, UpgradeType t, float cds, float v);
};

class Element : public Item {
    public:
        Animation *animate;
        char* hoverDialogue;
        ElementAction action;
        Rect dst;

        GameScene::DisplayElement *display;

        Element(Animation *A, Rect d, char *hd, ElementAction a, GameScene::DisplayElement *de);

        void pickup(Hero& hero, GameScene::DisplayState& currDisplay, GameScene::DisplayElement& currElement);
};

#endif