#ifndef alien_h_
#define alien_h_

#include <engine.h>
#include <hero.h>
#include <transform.h>

struct Animation {
    std::vector<Texture> frames;
    int no_frames;
    float duration;
    bool loop;
};

enum STATE_ID {IDLE, ANGRY, FOLLOW, JUMP, COOL};

struct AlienAdult {
    Transform transform;
    Vec2 vel;
    Vec2 size;
    Vec2 currentTarget = Vec2(0,0);
    Vec2 hitbox;
    float detecRadius;
    bool active;

    float start = 0.0f;
    float elapsed = 0.0f;
    float cooldown = 2;
    

    Texture texture;
    Animation animate;

    STATE_ID state;
};

struct Alien {
    Transform transform;
    Vec2 vel;
    Vec2 size;
    Vec2 currentTarget = Vec2(0,0);

    float detecRadius;
    bool active;

    float start = 0.0f;
    float elapsed = 0.0f;
    float cooldown = 2;

    Texture texture;
    Animation animate;

    STATE_ID state;
};

struct AlienRanged {
    Transform transform;
    Vec2 vel;
    Vec2 size;
    Vec2 currentTarget = Vec2(0,0);

    float detecRadius;
    bool active;

    Vec2 projectilePos;
    Vec2 projectileVel;
    float projectileRad = 10;
    bool shooting;

    float start = 0.0f;
    float elapsed = 0.0f;
    float cooldown = 1;

    Texture texture;
    Animation animate;

    STATE_ID state;
};




// check collsions between aliens
Alien* alienCollision(Vec2 pos, player &p1);

// create new alien
bool addAlien(std::vector<Alien> &Horde, Texture spritesheet);
bool addAlien(std::vector<AlienAdult> &Horde, Texture spritesheet);
bool addAlien(std::vector<AlienRanged> &Horde, Texture spritesheet);

// chase player
void chase(Alien &alien, player &p1, float dt);  // , float r = 0
void chase(AlienAdult &alien, player &p1, float dt);
void chase(AlienRanged &alien, player &p1, float dt);

// jumping
void jump(Alien &alien, Vec2 target, float dt);
void jump(AlienAdult &alien, Vec2 target, float dt);
void jump(AlienRanged &alien, Vec2 target, float dt);

// projectiles
void spit(AlienRanged &alien, Vec2 target, float dt);

// Finite state machine controller - remember state
void fsmAlien(std::vector<Alien> &Horde, player &p1, float dt, float start);
void fsmAlien(std::vector<AlienAdult> &Horde, player &p1, float dt, float start);
void fsmAlien(std::vector<AlienRanged> &Horde, player &p1, float dt, float start);

// draw alien
void drawAlien(Alien &alien, bool active, float start);
void drawAlien(AlienAdult &alien, bool active, float start);
void drawAlien(AlienRanged &alien, bool active, float start);

#endif