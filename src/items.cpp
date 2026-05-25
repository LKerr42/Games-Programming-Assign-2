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