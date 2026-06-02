#include <items.h>
#include <hero.h>
#include <gameScene.h>

#include <iostream>

// -- Weapon --
Weapon::Weapon(Animation *largeA, Texture *small, Rect d, char *hd, float fs, float rs, float fe) 
        : Item(largeA, small, d, hd) {
    //timers
    fireTimer = (Timer*)calloc(1, sizeof(Timer));
    activateTimer(fireTimer, fs, true, true);

    reloadTimer = (Timer*)calloc(1, sizeof(Timer));
    activateTimer(reloadTimer, rs, false, false);

    //energy
    currEnergy = fe;
    fullEnergy = fe;
};

void Weapon::pickup(Hero& hero, GameScene::DisplayState *currDisplay, GameScene::DisplayElement *currElement) {
    hero.currWeapon = this;
}

Item* Weapon::dropItem(Hero& hero) {
    if (hero.currWeapon == nullptr) return nullptr;
    Item* dropped = hero.currWeapon;
    dropped->dst.x = hero.transform.getPosition().x;
    dropped->dst.y = hero.transform.getPosition().y;
    
    hero.currWeapon = nullptr;
    return dropped;
}

// -- Armour --
Armour::Armour(Animation *largeA, Texture *small, Rect d, char *hd, float r, float hu) 
        : Item(largeA, small, d, hd){
    resistance = r;
    healthUpgrade = hu;

    this->animationLarge = animationLarge;
};

void Armour::pickup(Hero& hero, GameScene::DisplayState *currDisplay, GameScene::DisplayElement *currElement) {
    hero.currArmour = this;
}

Item* Armour::dropItem(Hero& hero) {
    if (hero.currArmour == nullptr) return nullptr;
    Item* dropped = hero.currArmour;
    dropped->dst.x = hero.transform.getPosition().x;
    dropped->dst.y = hero.transform.getPosition().y;

    hero.currArmour = nullptr;
    return dropped;
}

// -- Upgrade --
Upgrade::Upgrade(Animation *largeA, Texture *small, Rect d, char *hd, UpgradeType t, float cds, float v)
        : Item(largeA, small, d, hd) {
    //timer
    cooldown  = (Timer*)calloc(1, sizeof(Timer));
    activateTimer(cooldown, cds, true, true);

    //type and value
    type = t;
    value = v;
};

void Upgrade::pickup(Hero& hero, GameScene::DisplayState *currDisplay, GameScene::DisplayElement *currElement) {
    hero.currUpgrade = this;
}

Item* Upgrade::dropItem(Hero& hero) {
    if (hero.currUpgrade == nullptr) return nullptr;
    Item* dropped = hero.currUpgrade;
    dropped->dst.x = hero.transform.getPosition().x;
    dropped->dst.y = hero.transform.getPosition().y;

    hero.currUpgrade = nullptr;
    return dropped;
}

// -- Element --
Element::Element(Animation *largeA, Rect d, char *hd, GameScene::DisplayElement *de) 
        : Item(largeA, nullptr, d, hd) {
    element = de;
}

void Element::pickup(Hero& hero, GameScene::DisplayState *currDisplay, GameScene::DisplayElement *currElement) {
    *currDisplay = (GameScene::DisplayState)1;
    *currElement = *this->element;
}

Item* Element::dropItem(Hero& hero) {
    return this;
}

//-- item def --
std::vector<ItemDefinition> levelOneDef;
std::vector<ItemDefinition> levelTwoDef;
std::vector<ItemDefinition> levelThreeDef;

void loadDefinitions() {
    // -- level one --
    levelOneDef.push_back(
        (ItemDefinition){
            .smallTexture = new Texture(loadTexture("./assets/images/items/weapon_small_01.png")),
            .animationLarge = new Animation(loadAnimation("./assets/images/items/weapon_large_animation_grid.png", (Rect){0, 0, 20, 20}, 6, 2.0f, true)),
            .data = {0.25f, 2.0f, 100},
            .dst = (Rect){640, WINDOW_HEIGHT/2, 0, 0},

            .ele = nullptr,
            .upType = NONE,

            .title = "Weapon"
        }
    );
    levelOneDef.push_back(
        (ItemDefinition){
            .smallTexture = new Texture(loadTexture("./assets/images/items/armour_small_01.png")),
            .animationLarge = new Animation(loadAnimation("./assets/images/items/armour_large_animation_grid.png", (Rect){0, 0, 20, 20}, 6, 2.0f, true)),
            .data = {100, 100},
            .dst = (Rect){700, WINDOW_HEIGHT/2, 0, 0},

            .ele = nullptr,
            .upType = NONE,

            .title = "Armour"
        }
    );
    levelOneDef.push_back(
        (ItemDefinition){
            .smallTexture = new Texture(loadTexture("./assets/images/items/upgrade_small_01.png")),
            .animationLarge = new Animation(loadAnimation("./assets/images/items/upgrade_large_animation_grid.png", (Rect){0, 0, 20, 20}, 6, 2.0f, true)),
            .data = {0, 2.0f, 2.0f},
            .dst = (Rect){400, WINDOW_HEIGHT/2, 0, 0},

            .ele = nullptr,
            .upType = DASH,

            .title = "Upgrade"
        }
    );
    levelOneDef.push_back(
        (ItemDefinition){
            .smallTexture = nullptr,
            .animationLarge = new Animation(loadAnimation("./assets/images/items/letter_animation_grid.png", (Rect){0, 0, 20, 20}, 6, 2.0f, true)),
            .data = {},
            .dst = (Rect){300, WINDOW_HEIGHT/2, 0, 0},

            .ele = new GameScene::DisplayElement((GameScene::DisplayElement){
                "Hmm... seems to be left here by the crew",
                25,
                new Texture(loadTexture("./assets/images/letter_large.png"))
            }),
            .upType = NONE,

            .title = "Element"
        }
    );

    // -- level two --
}

void loadLevel(GameScene::Level toLoad, std::list<Item*>& itemsList) {
    for (Item* item : itemsList) {
        delete item;
    }
    itemsList.clear();

    std::vector<ItemDefinition> *definitons;
    if (toLoad == GameScene::LEVEL_ONE) {
        definitons = &levelOneDef;
    } else if (toLoad == GameScene::LEVEL_TWO) {
        definitons = &levelTwoDef;
    } else {
        definitons = &levelThreeDef;
    }

    for (ItemDefinition def : *definitons) {
        if (def.title == "Weapon") {
            itemsList.push_back(new Weapon(
                def.animationLarge, def.smallTexture, 
                def.dst, "'E' to grab", 
                def.data[0], def.data[1], def.data[2]
            ));
        } else if (def.title == "Armour") {
            itemsList.push_back(new Armour(
                def.animationLarge, def.smallTexture, 
                def.dst, "'E' to grab", 
                def.data[0], def.data[1]
            ));
        } else if (def.title == "Upgrade") {
            itemsList.push_back(new Upgrade(
                def.animationLarge, def.smallTexture, 
                def.dst, "'E' to grab", 
                def.upType, def.data[0], def.data[1]
            ));
        } else {
            itemsList.push_back(new Element(
                def.animationLarge, 
                def.dst, "'E' to grab", 
                def.ele
            ));
        }
    }
}