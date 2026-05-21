#include <items.h>
#include <hero.h>

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