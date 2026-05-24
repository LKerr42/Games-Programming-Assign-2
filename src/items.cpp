#include <items.h>
#include <hero.h>

// -- Weapon --
Weapon::Weapon(Texture *large, Texture *small, Rect d, float fs, float rs, float fe) : Item(large, small, d) {
    //timers
    fireTimer = (Timer*)calloc(1, sizeof(Timer));
    activateTimer(fireTimer, fs, true, true);

    reloadTimer = (Timer*)calloc(1, sizeof(Timer));
    activateTimer(reloadTimer, rs, false, false);

    //energy
    currEnergy = fe;
    fullEnergy = fe;
};

void Weapon::pickup(Hero& hero) {
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
    Armour::Armour(Texture *large, Texture *small, Rect d, float r, float hu) : Item(large, small, d){
        resistance = r;
        healthUpgrade = hu;
    };

void Armour::pickup(Hero& hero) {
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
Upgrade::Upgrade(Texture *large, Texture *small, Rect d, UpgradeType t, float cds, float v) : Item(large, small, d) {
    //timer
    cooldown  = (Timer*)calloc(1, sizeof(Timer));
    activateTimer(cooldown, cds, true, true);

    //type and value
    type = t;
    value = v;
};

void Upgrade::pickup(Hero& hero) {
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