#ifndef ITEMS_H_
#define ITEMS_H_

#include <engine.h>

#include <animations.h>

#include <vector>
#include <list>

namespace GameScene {
    typedef struct DisplayElement DisplayElement;
    enum DisplayState : int;
    enum Level : int;
}

class Hero;

enum UpgradeType {
    NONE,
    DASH,
    MELEE,
    INVISIBILITY,
    LONGER_SIGHT
};

// -- polymorphic items -
class Item {
    public:
        Texture *smallTexture;
        Animation *animationLarge;
        Rect dst;
        char *hoverDialogue;

        Item(Animation *largeA, Texture *small, Rect d, char *hd) 
                : animationLarge(largeA), smallTexture(small), dst(d), hoverDialogue(hd) {
            dst.width = dst.height = 40;
        };

        virtual void pickup(Hero& hero, GameScene::DisplayState *currDisplay, GameScene::DisplayElement *currElement) = 0;
        virtual Item* dropItem(Hero& hero) = 0;
        virtual ~Item() = default;
};

class Weapon : public Item {
    public:
        float currEnergy, fullEnergy;
        Timer *fireTimer, *reloadTimer;

        Weapon(Animation *largeA, Texture *small, Rect d, char *hd, float fs, float rs, float fe);

        void pickup(Hero& hero, GameScene::DisplayState *currDisplay, GameScene::DisplayElement *currElement) override;
        Item* dropItem(Hero& hero) override;
};

class Armour : public Item {
    public:
        float resistance, healthUpgrade;

        Armour(Animation *largeA, Texture *small, Rect d, char *hd, float r, float hu);

        void pickup(Hero& hero, GameScene::DisplayState *currDisplay, GameScene::DisplayElement *currElement) override;
        Item* dropItem(Hero& hero) override;
};

class Upgrade : public Item {
    public:
        UpgradeType type;
        Timer *cooldown;
        float value;

        Upgrade(Animation *largeA, Texture *small, Rect d, char *hd, UpgradeType t, float cds, float v);

        void pickup(Hero& hero, GameScene::DisplayState *currDisplay, GameScene::DisplayElement *currElement) override;
        Item* dropItem(Hero& hero) override;
};

class Element : public Item {
    public:
        GameScene::DisplayElement *element;

        Element(Animation *largeA, Rect d, char *hd, GameScene::DisplayElement *de);

        void pickup(Hero& hero, GameScene::DisplayState *currDisplay, GameScene::DisplayElement *currElement) override;
        Item* dropItem(Hero& hero) override;
};


// -- init --
typedef struct ItemDefinition {
    Texture *smallTexture;
    Animation *animationLarge;
    std::vector<float> data;
    Rect dst;

    GameScene::DisplayElement* ele;
    UpgradeType upType;

    char* title;
} ItemDefinition;

void loadDefinitions();
void loadLevel(GameScene::Level toLoad, std::list<Item*>& itemsList);

#endif