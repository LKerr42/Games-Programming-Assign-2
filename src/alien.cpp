#include <alien.h>
#include <hero.h>
#include <collision.h>
#include <vector>
#include <iostream>

// hatchling -----------------------------------------------------------------------------------------------------
Alien* alienCollision(Vec2 pos, Hero &p1) {
    
}

bool addAlien(std::vector<Alien> &Horde, Texture spritesheet) {
    Alien alien;

    int wall = 0;
    // int wall = uniform(0,4);
    if(wall == 0) {
        alien.transform.pos = Vec2(uniform(0, WINDOW_WIDTH), -10);   
    } else if(wall == 1) {
        alien.transform.pos = Vec2(uniform(0, WINDOW_WIDTH), WINDOW_HEIGHT+10);  
    } else if(wall == 2) {
        alien.transform.pos = Vec2(-10, uniform(0, WINDOW_HEIGHT));
    } else if(wall == 3) {
        alien.transform.pos = Vec2(WINDOW_WIDTH+10, uniform(0, WINDOW_HEIGHT));
    }


    alien.size = Vec2(40, 40);

    alien.state = IDLE;
    alien.active = true;
    alien.texture = subTexture(spritesheet, {0, 40, 40, 40});

    alien.animate.frames.push_back(subTexture(spritesheet, {40, 40, 40, 40}));
    alien.animate.frames.push_back(subTexture(spritesheet, {0, 40, 40, 40}));
    alien.animate.frames.push_back(subTexture(spritesheet, {80, 40, 40, 40}));
    alien.animate.frames.push_back(subTexture(spritesheet, {0, 40, 40, 40}));
    alien.animate.no_frames = 4;
    alien.animate.duration = 0.5;

    bool hasSpace = true;
    for(int j = 0; j < Horde.size(); j++) {
        if(collision(alien.transform.position(), alien.size, alien.transform.localAngle, Horde[j].transform.position(), Horde[j].size, Horde[j].transform.localAngle)) {
            hasSpace = false;
        }
    }

    if(hasSpace) {
        Horde.push_back(alien);
        return true;
    }
    // Return result
    return false;
}

void chase(Alien &alien, player &p1, float dt) {
    Vec2 toPlayer = unit(p1.position - alien.transform.position());


    float targetAngle = atan2(toPlayer.y, toPlayer.x) / M_PI * 180.0f - 270;
    alien.transform.localAngle = targetAngle;

    alien.vel = toPlayer * 150;
    alien.transform.pos += alien.vel * dt;
}

void jump(Alien &alien, Vec2 target, float dt) {
    if(distance(alien.transform.pos, target) < 10) {
        alien.active = true;
        return;
    }
    Vec2 toPlayer = unit(target - alien.transform.position());
    float targetAngle = atan2(toPlayer.y, toPlayer.x) / M_PI * 180.0f - 270;
    alien.transform.localAngle = targetAngle;

    
    alien.vel = toPlayer * 500;
    alien.transform.pos += alien.vel * dt;
}

void fsmAlien(std::vector<Alien> &Horde, player &p1, float dt, float start) {
    for(int i = 0; i < Horde.size(); i++) {

         //else {Horde[i].state.id == IDLE;}
        
        if(Horde[i].state == JUMP) {
            jump(Horde[i], Horde[i].currentTarget, dt);
            //printf("is in jumping state\n");
            
            if(distance(Horde[i].transform.pos, Horde[i].currentTarget) < 10) {
                printf("has reached target after jump\n");
                Horde[i].currentTarget = Vec2(0,0);
                Horde[i].state = COOL;
                Horde[i].start = getTimeInSeconds();
            }
        } else if(Horde[i].state == COOL) {
            //printf("cooling off\n");
            float current = getTimeInSeconds();
            Horde[i].elapsed = current - Horde[i].start;
            if(Horde[i].elapsed > Horde[i].cooldown) {
                printf("cooling ended, elapsed: %f \n", Horde[i].elapsed);
                Horde[i].elapsed = 0.0f;
                Horde[i].state = ANGRY;
            } else {chase(Horde[i], p1, dt);}

            // time setter to wait or chase -- after timer if in range jump again
        } else if(Horde[i].state == ANGRY) {
            chase(Horde[i], p1, dt);
            //printf("is in chase state\n");
            if(distance(Horde[i].transform.pos, p1.position) < p1.innerRad) {
                Horde[i].currentTarget = p1.position;
                Horde[i].state = JUMP;
            }
            
        } 
        else if(distance(Horde[i].transform.pos, p1.position) < p1.outerRad) {
            Horde[i].state = ANGRY;
            
        }
        
        
    }
}

void drawAlien(Alien &alien, bool active, float start) {
    if(active) {
        float current = getTimeInSeconds();
        float elapsed = current - start;
        int frameIndex = (elapsed / alien.animate.duration) * alien.animate.no_frames;
        frameIndex %= alien.animate.no_frames;
        drawTexture(alien.animate.frames[frameIndex], alien.transform.pos - alien.size/2, alien.size, alien.transform.localAngle);
    } else {
        drawTexture(alien.texture, alien.transform.pos - alien.size/2, alien.size, alien.transform.localAngle);
    }
    drawRect(alien.transform.pos - alien.size/2, alien.size, Color::red, alien.transform.localAngle);
}



// mature ALien -------------------------------------------------------------------------------------------------------------------------------
bool addAlien(std::vector<AlienAdult> &Horde, Texture spritesheet) {
    AlienAdult alien;

    int wall = 0;
    // int wall = uniform(0,4);
    if(wall == 0) {
        alien.transform.pos = Vec2(uniform(0, WINDOW_WIDTH), -10);   
    } else if(wall == 1) {
        alien.transform.pos = Vec2(uniform(0, WINDOW_WIDTH), WINDOW_HEIGHT+10);  
    } else if(wall == 2) {
        alien.transform.pos = Vec2(-10, uniform(0, WINDOW_HEIGHT));
    } else if(wall == 3) {
        alien.transform.pos = Vec2(WINDOW_WIDTH+10, uniform(0, WINDOW_HEIGHT));
    }

    alien.size = Vec2(80, 80);
    alien.hitbox = Vec2(80, 80);
    alien.state = IDLE;
    alien.active = true;
    alien.texture = subTexture(spritesheet, {0, 0, 60, 60});

    alien.animate.frames.push_back(subTexture(spritesheet, {60, 0, 60, 60}));
    alien.animate.frames.push_back(subTexture(spritesheet, {0, 0, 60, 60}));
    alien.animate.frames.push_back(subTexture(spritesheet, {120, 0, 60, 60}));
    alien.animate.frames.push_back(subTexture(spritesheet, {0, 0, 60, 60}));
    alien.animate.no_frames = 4;
    alien.animate.duration = 0.5;

    bool hasSpace = true;
    for(int j = 0; j < Horde.size(); j++) {
        if(collision(alien.transform.position(), alien.size, alien.transform.localAngle, Horde[j].transform.position(), Horde[j].size, Horde[j].transform.localAngle)) {
            hasSpace = false;
        }
    }

    if(hasSpace) {
        Horde.push_back(alien);
        return true;
    }
    // Return result
    return false;
}

// check if player has entered alien awareness
bool awareOfPlayer(AlienAdult &alien, player &p1) {
    if(distance(alien.transform.pos, p1.position) < p1.outerRad) {
        return true;
    } else {
        return false;
    }
}

// chase player
void chase(AlienAdult &alien, player &p1, float dt) {  // , float r = 0

    Vec2 toPlayer = unit(p1.position - alien.transform.position());


    float targetAngle = atan2(toPlayer.y, toPlayer.x) / M_PI * 180.0f - 270;
    alien.transform.localAngle = targetAngle;

    alien.vel = toPlayer * 80;
    alien.transform.pos += alien.vel * dt;
}

// jumping
void jump(AlienAdult &alien, Vec2 target, float dt) {
    if(distance(alien.transform.pos, target) < 10) {
        alien.active = true;
        return;
    }
    Vec2 toPlayer = unit(target - alien.transform.position());
    float targetAngle = atan2(toPlayer.y, toPlayer.x) / M_PI * 180.0f - 270;
    alien.transform.localAngle = targetAngle;
    alien.hitbox = Vec2(80, 120);


    
    alien.vel = toPlayer * 300;
    alien.transform.pos += alien.vel * dt;
}

// Finite state machine controller - remember state
void fsmAlien(std::vector<AlienAdult> &Horde, player &p1, float dt, float start) {

    for(int i = 0; i < Horde.size(); i++) {
        //chase(Horde[i], p1, dt);
        if(Horde[i].state == JUMP) {
            jump(Horde[i], Horde[i].currentTarget, dt);
            //printf("is in jumping state\n");
            
            if(distance(Horde[i].transform.pos, Horde[i].currentTarget) < 10) {
                printf("has reached target after jump\n");
                Horde[i].hitbox = Vec2(80,80);
                Horde[i].currentTarget = Vec2(0,0);
                Horde[i].state = COOL;
                Horde[i].start = getTimeInSeconds();
            }
        } else if(Horde[i].state == COOL) {
            //printf("cooling off\n");
            float current = getTimeInSeconds();
            Horde[i].elapsed = current - Horde[i].start;
            if(Horde[i].elapsed > Horde[i].cooldown) {
                printf("cooling ended, elapsed: %f \n", Horde[i].elapsed);
                Horde[i].elapsed = 0.0f;
                Horde[i].state = ANGRY;
            } else {chase(Horde[i], p1, dt);}

            // time setter to wait or chase -- after timer if in range jump again
        } else if(Horde[i].state == ANGRY) {
            chase(Horde[i], p1, dt);
            //printf("is in chase state\n");
            if(distance(Horde[i].transform.pos, p1.position) < p1.innerRad) {
                Horde[i].currentTarget = p1.position;
                Horde[i].state = JUMP;
            }
            
        } 
        else if(distance(Horde[i].transform.pos, p1.position) < p1.outerRad) {
            Horde[i].state = ANGRY;
            
        }

    }
}

void drawAlien(AlienAdult &alien, bool active, float start) {
    if(active) {
        float current = getTimeInSeconds();
        float elapsed = current - start;
        int frameIndex = (elapsed / alien.animate.duration) * alien.animate.no_frames;
        frameIndex %= alien.animate.no_frames;
        drawTexture(alien.animate.frames[frameIndex], alien.transform.pos - alien.size/2, alien.size, alien.transform.localAngle);
    } else {
        drawTexture(alien.texture, alien.transform.pos - alien.size/2, alien.size, alien.transform.localAngle);
    }
    drawRect(alien.transform.pos - alien.hitbox/2, alien.hitbox, Color::red, alien.transform.localAngle);

}


// alien ranged -----------------------------------------------------------------------------------------------------------------
bool addAlien(std::vector<AlienRanged> &Horde, Texture spritesheet) {
    AlienRanged alien;

    int wall = 0;
    // int wall = uniform(0,4);
    if(wall == 0) {
        alien.transform.pos = Vec2(uniform(0, WINDOW_WIDTH), -10);   
    } else if(wall == 1) {
        alien.transform.pos = Vec2(uniform(0, WINDOW_WIDTH), WINDOW_HEIGHT+10);  
    } else if(wall == 2) {
        alien.transform.pos = Vec2(-10, uniform(0, WINDOW_HEIGHT));
    } else if(wall == 3) {
        alien.transform.pos = Vec2(WINDOW_WIDTH+10, uniform(0, WINDOW_HEIGHT));
    }

    alien.size = Vec2(60, 60);

    alien.state = IDLE;
    alien.active = true;
    alien.texture = subTexture(spritesheet, {0, 0, 60, 60});

    alien.animate.frames.push_back(subTexture(spritesheet, {40, 0, 40, 40}));
    alien.animate.frames.push_back(subTexture(spritesheet, {0, 0, 40, 40}));
    alien.animate.frames.push_back(subTexture(spritesheet, {80, 0, 40, 40}));
    alien.animate.frames.push_back(subTexture(spritesheet, {0, 0, 40, 40}));
    alien.animate.no_frames = 4;
    alien.animate.duration = 0.5;

    bool hasSpace = true;
    for(int j = 0; j < Horde.size(); j++) {
        if(collision(alien.transform.position(), alien.size, alien.transform.localAngle, Horde[j].transform.position(), Horde[j].size, Horde[j].transform.localAngle)) {
            hasSpace = false;
        }
    }

    if(hasSpace) {
        Horde.push_back(alien);
        return true;
    }
    // Return result
    return false;
}

// chase player
void chase(AlienRanged &alien, player &p1, float dt) {  // , float r = 0

    Vec2 toPlayer = unit(p1.position - alien.transform.position());


    float targetAngle = atan2(toPlayer.y, toPlayer.x) / M_PI * 180.0f - 270;
    alien.transform.localAngle = targetAngle;

    alien.vel = toPlayer * 80;
    alien.transform.pos += alien.vel * dt;
}

// jumping
void jump(AlienRanged &alien, Vec2 target, float dt) {
    if(distance(alien.transform.pos, target) < 10) {
        alien.active = true;
        return;
    }
    Vec2 toPlayer = unit(target - alien.transform.position());
    float targetAngle = atan2(toPlayer.y, toPlayer.x) / M_PI * 180.0f - 270;
    alien.transform.localAngle = targetAngle;

    alien.vel = toPlayer * -700;
    alien.transform.pos += alien.vel * dt;
}

void spit(AlienRanged &alien, Vec2 target, float dt) {
    Vec2 toPlayer = unit(target - alien.transform.position());
    alien.projectileVel = toPlayer * 350;
    alien.projectilePos += alien.projectileVel * dt;
}

// Finite state machine controller - remember state
void fsmAlien(std::vector<AlienRanged> &Horde, player &p1, float dt, float start) {

    for(int i = 0; i < Horde.size(); i++) {

        if(Horde[i].state == JUMP) {
            jump(Horde[i], Horde[i].currentTarget, dt);

            
            if(distance(Horde[i].transform.pos, Horde[i].currentTarget) > 300) {
                printf("has reached target after jump\n");

                Horde[i].state = COOL;
                Horde[i].projectilePos = Horde[i].transform.pos;
                Horde[i].start = getTimeInSeconds();
            }
        } else if(Horde[i].state == COOL) {

            float current = getTimeInSeconds();
            Horde[i].elapsed = current - Horde[i].start;
            
            if(Horde[i].elapsed > Horde[i].cooldown) {
                printf("cooling ended, elapsed: %f \n", Horde[i].elapsed);
                Horde[i].elapsed = 0.0f;
                Horde[i].state = ANGRY;
                Horde[i].shooting = false;
            } else {Horde[i].shooting = true;}

        } else if(Horde[i].state == ANGRY) {
            chase(Horde[i], p1, dt);

            if(distance(Horde[i].transform.pos, p1.position) < p1.innerRad) {
                Horde[i].currentTarget = p1.position;
                Horde[i].state = JUMP;
            }
            
        } 
        else if(distance(Horde[i].transform.pos, p1.position) < p1.outerRad) {
            Horde[i].state = ANGRY;
            
        }

        if(Horde[i].shooting) {
            spit(Horde[i], Horde[i].currentTarget, dt);
        }

    }
}

void drawAlien(AlienRanged &alien, bool active, float start) {
    if(active) {
        float current = getTimeInSeconds();
        float elapsed = current - start;
        int frameIndex = (elapsed / alien.animate.duration) * alien.animate.no_frames;
        frameIndex %= alien.animate.no_frames;
        drawTexture(alien.animate.frames[frameIndex], alien.transform.pos - alien.size/2, alien.size, alien.transform.localAngle);
    } else {
        drawTexture(alien.texture, alien.transform.pos - alien.size/2, alien.size, alien.transform.localAngle);
    }
    drawRect(alien.transform.pos - alien.size/2, alien.size, Color::red, alien.transform.localAngle);

    if(alien.shooting) {
        fillCircle(alien.projectilePos, alien.projectileRad, Color::green);
    }

}
