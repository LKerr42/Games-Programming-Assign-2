#include <items.h>
#include <hero.h>
#include <gameScene.h>

#include <iostream>

// -- Weapon --
Weapon:: Weapon(Texture *small, float fs, float rs, float fe) 
        : Item(small) {
    //timers
    fireTimer = (Timer*)calloc(1, sizeof(Timer));
    activateTimer(fireTimer, fs, true, true);

    reloadTimer = (Timer*)calloc(1, sizeof(Timer));
    activateTimer(reloadTimer, rs, false, false);

    //energy
    currEnergy = fe;
    fullEnergy = fe;
};

// -- Armour --
Armour::Armour(Texture *small, float r, float hu) 
        : Item(small){
    resistance = r;
    healthUpgrade = hu;
}

// -- Upgrade --
Upgrade::Upgrade(Texture *small, UpgradeType t, float cds, float v)
        : Item(small) {
    //timer
    cooldown  = (Timer*)calloc(1, sizeof(Timer));
    activateTimer(cooldown, cds, true, true);

    //type and value
    type = t;
    value = v;
};

// -- Element --
Element::Element(Animation *A, Rect d, char *hd, ElementAction a, GameScene::DisplayElement *de) 
        : Item(nullptr) {
    animate = A;
    action = a;
    display = de;
    dst = d;
    hoverDialogue = hd;
}

void Element::pickup(Hero& hero, GameScene::DisplayState& currDisplay, GameScene::DisplayElement& currElement) {
    switch (this->action) {
        case SHOW_DISPLAY: {
            currDisplay = GameScene::DisplayState::ELEMENT;
            currElement = *this->display;
            break;
        }
        case OPEN_WORKBENCH: {
            currDisplay = GameScene::DisplayState::WORKBENCH;
            break;
        }
        case START_HORDE: {
            break;
        }
        case HEAL_HERO: {
            break;
        }
    }
}