#include <alien.h>
#include <hero.h>
#include <collisions.h>
#include <vector>
#include <iostream>
#include <algorithm>
#include <gameScene.h>
#include <sound.h>

// hatchling -----------------------------------------------------------------------------------------------------
void alienCollision(std::vector<Alien> &Horde) {
    std::vector<Alien*> aliens;

    for(Alien a : Horde) {
        Alien *b = &a;
        aliens.push_back(b);
    }

    /*std:sort(aliens.begin(), aliens.end(), [](const auto &a, const auto &b){
        if(a->transform.getBoundingBox().x != b->transform.getBoundingBox().x) return a->transform.getBoundingBox().x < b->transform.getBoundingBox().x;
    });*/

    for(int i = 0; i < aliens.size(); i++) {
        Alien *a1 = aliens[i];
        for(int j = i + 1; j < aliens.size(); j++) {
            Alien *a2 = aliens[j];

            if(a2->transform.getBoundingBox().x > a1->transform.getBoundingBox().x + a1->transform.getBoundingBox().width) break;

            // bool collision(Vec2 pos1, Vec2 size1, float angle1, Vec2 pos2, Vec2 size2, float angle2)

            Vec2 pos1 = Vec2(a1->transform.getBoundingBox().x, a1->transform.getBoundingBox().y);
            Vec2 size1 = Vec2(a1->transform.getBoundingBox().width, a1->transform.getBoundingBox().height);
            Vec2 pos2 = Vec2(a2->transform.getBoundingBox().x, a2->transform.getBoundingBox().y);
            Vec2 size2 = Vec2(a2->transform.getBoundingBox().width, a2->transform.getBoundingBox().height);

            if(collision(pos1, size1, 0, pos2, size2, 0)) {
                printf("Horde[i].pos: %f, %f  Horde[j].pos: %f, %f\n", Horde[i].transform.getBoundingBox().x, Horde[i].transform.getBoundingBox().y, Horde[j].transform.getBoundingBox().x, Horde[i].transform.getBoundingBox().y);
                printf("alien[i].pos: %f, %f  alien[j].pos: %f, %f\n", aliens[i]->transform.getBoundingBox().x, aliens[i]->transform.getBoundingBox().y, aliens[j]->transform.getBoundingBox().x, aliens[j]->transform.getBoundingBox().y);
                Rect alienBox = a1->transform.getBoundingBox();
                float left = (alienBox.x + alienBox.width) - a2->transform.getBoundingBox().x;
                float right = (a2->transform.getBoundingBox().x +  a2->transform.getBoundingBox().width) - alienBox.x;
                float top = (alienBox.y + alienBox.height) - a2->transform.getBoundingBox().y;
                float bottom = (a2->transform.getBoundingBox().y +  a2->transform.getBoundingBox().height) - alienBox.y;

                float minOverlap = min(min(left, right), min(top, bottom));

                if (minOverlap == left) {
                    a1->transform.translate(Vec2(-left, 0));
                } else if (minOverlap == right) {
                    a1->transform.translate(Vec2(right, 0));
                } else if (minOverlap == top) {
                    a1->transform.translate(Vec2(0, -top));
                } else if (minOverlap == bottom) {
                    a1->transform.translate(Vec2(0, bottom));
                }
            }
        }
    }
}

bool wallCollisions(Alien &alien, std::vector<Rect> &walls) {
    bool CheckingCollision = false;

    for(int i = 0; i < walls.size(); i++) {
        Rect wall1 = walls[i];

            //bool collision(Vec2 pos1, Vec2 size1, float angle1, Vec2 pos2, Vec2 size2, float angle2)
            Vec2 pos1 = Vec2(alien.transform.getBoundingBox().x, alien.transform.getBoundingBox().y);
            Vec2 size1 = Vec2(alien.transform.getBoundingBox().width, alien.transform.getBoundingBox().height);
            Vec2 pos2 = Vec2(wall1.x, wall1.y);
            Vec2 size2 = Vec2(wall1.width, wall1.height);
            CheckingCollision = collision(pos1, size1, 0, pos2, size2, 0);
        
        if(CheckingCollision) {
            //printf("Alien box: %f, %f    Wall: %f, %f \n", pos1.x, pos1.y, pos2.x, pos2.y);
            Rect alienBox = alien.transform.getBoundingBox();
            float left = (alienBox.x + alienBox.width) - wall1.x;
            float right = (wall1.x +  wall1.width) - alienBox.x;
            float top = (alienBox.y + alienBox.height) - wall1.y;
            float bottom = (wall1.y +  wall1.height) - alienBox.y;

            float minOverlap = min(min(left, right), min(top, bottom));

            if (minOverlap == left) {
                alien.transform.translate(Vec2(-left, 0));
            } else if (minOverlap == right) {
                alien.transform.translate(Vec2(right, 0));
            } else if (minOverlap == top) {
                alien.transform.translate(Vec2(0, -top));
            } else if (minOverlap == bottom) {
                alien.transform.translate(Vec2(0, bottom));
            }
        }

    }
    return CheckingCollision;
}

void laserCollision(Alien &alien, std::vector<GameScene::Laser> &lasers, audioClips &audioObject) {
    bool collided = false;

    for(int i = 0; i < lasers.size(); i++) {
        GameScene::Laser &bullet = lasers[i];

        /*Vec2 pos1 = Vec2(alien.transform.getBoundingBox().x, alien.transform.getBoundingBox().y);
        Vec2 size1 = Vec2(alien.transform.getBoundingBox().width, alien.transform.getBoundingBox().height);
        Vec2 pos2 = Vec2(bullet.pos.x, bullet.pos.y);
        Vec2 size2 = bullet.getSize();*/
        //bool collision(Vec2 pos1, Vec2 size1, float angle1, Vec2 pos2, Vec2 size2, float angle2);
        collided = collision(alien.transform.pos - alien.transform.size/2, alien.transform.size, alien.transform.angle, bullet.transform.pos - bullet.transform.size/2, bullet.transform.size, bullet.transform.angle);
        if(collided) {
            printf("bullet hit \n");
            alien.health -= 25;
            playLaserHit(audioObject);
            deleteLazer(bullet, i);

        }
    }
}

void heroCollision(Alien &alien, Hero &p1, float dt) {
    Vec2 toPlayer = unit(p1.transform.pos - alien.transform.pos);
    float targetAngle = atan2(toPlayer.y, toPlayer.x) / M_PI * 180.0f - 270;
    alien.transform.angle = targetAngle;

    alien.vel = toPlayer * - 200;
    alien.transform.pos += alien.vel * dt;

    p1.transform.pos += (toPlayer * 100) * dt;
}

bool addAlien(std::vector<Alien> &Horde, Texture spritesheet, AlienType type) {
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

    alien.type = type;
    switch(type) {
        case HATCHLING:
            alien.transform.size = Vec2(40,40);
            alien.texture = subTexture(spritesheet, {0, 70, 20, 20});
            alien.animate.frames.push_back(subTexture(spritesheet, {0, 70, 20, 20}));
            alien.animate.frames.push_back(subTexture(spritesheet, {20, 70, 20, 20}));
            alien.animate.frames.push_back(subTexture(spritesheet, {0, 70, 20, 20}));
            alien.animate.frames.push_back(subTexture(spritesheet, {40, 70, 20, 20}));
            alien.animate.duration = 0.3f;
            alien.cooldown = 2;
            alien.health = 100;
            break;
        case MATURE:
            alien.transform.size = Vec2(80, 80);
            alien.texture = subTexture(spritesheet, {0, 0, 40, 40});
            alien.animate.frames.push_back(subTexture(spritesheet, {0, 0, 40, 40}));
            alien.animate.frames.push_back(subTexture(spritesheet, {40, 0, 40, 40}));
            alien.animate.frames.push_back(subTexture(spritesheet, {0, 0, 40, 40}));
            alien.animate.frames.push_back(subTexture(spritesheet, {80, 0, 40, 40}));
            alien.health = 200;
            alien.animate.duration = 0.75f;
            alien.cooldown = 3;
            break;
        case SPITTER:
            alien.transform.size = Vec2(60, 60);
            alien.texture = subTexture(spritesheet, {0, 40, 30, 30});
            alien.animate.frames.push_back(subTexture(spritesheet, {0, 40, 30, 30}));
            alien.animate.frames.push_back(subTexture(spritesheet, {30, 40, 30, 30}));
            alien.animate.frames.push_back(subTexture(spritesheet, {0, 40, 30, 30}));
            alien.animate.frames.push_back(subTexture(spritesheet, {60, 40, 30, 30}));
            alien.animate.duration = 0.5f;
            alien.cooldown = 1;
            
            alien.health = 150;
            break;
    }

    alien.state = IDLE;
    alien.active = true;

    alien.animate.numFrames = 4;
    //alien.animate.duration = 0.5f;
    alien.animate.start = getTimeInSeconds();

    Horde.push_back(alien);
    return true;

    // bool hasSpace = true;
    // for(int j = 0; j < Horde.size(); j++) {
    //     if (collision(alien.transform.pos, alien.size, alien.transform.angle, 
    //             Horde[j].transform.pos, Horde[j].size, Horde[j].transform.angle)) {
    //         hasSpace = false;
    //     }
    // }

    // if(hasSpace) {
    //     Horde.push_back(alien);
    //     return true;
    // }
    // // Return result
    // return false;
}

void chase(Alien &alien, Hero &p1, float dt) {
    int speed;
    if(alien.type == HATCHLING) speed = 100;
    if(alien.type == MATURE || alien.type == SPITTER) speed = 80;

    Vec2 toPlayer = unit(p1.transform.pos - alien.transform.pos);


    float targetAngle = atan2(toPlayer.y, toPlayer.x) / M_PI * 180.0f - 270;
    alien.transform.angle = targetAngle;

    alien.vel = toPlayer * speed;
    alien.transform.pos += alien.vel * dt;
    //alien.active = true;
}

void jump(Alien &alien, Vec2 target, float dt) {
    int speed;
    if(alien.type == HATCHLING) {speed = 500;}
    if(alien.type == MATURE) {speed = 300; alien.hitbox = Vec2(80, 120);} 
    if(alien.type == SPITTER) {speed = -600;}

    if(distance(alien.transform.pos, target) < 10) {
        alien.active = true;
        return;
    }
    Vec2 toPlayer = unit(target - alien.transform.pos);
    float targetAngle = atan2(toPlayer.y, toPlayer.x) / M_PI * 180.0f - 270;
    alien.transform.angle = targetAngle;

    
    alien.vel = toPlayer * speed;
    alien.transform.pos += alien.vel * dt;
}

void spit(Alien &alien, Vec2 target, float dt) {
    Vec2 toPlayer = unit(target - alien.transform.pos);
    alien.projectileVel = toPlayer * 350;
    alien.projectilePos += alien.projectileVel * dt;
}

void fsmAlien(std::vector<Alien> &Horde, std::vector<Rect> &walls, std::vector<GameScene::Laser> &lasers, Hero &p1, audioClips &audioObject, float dt, float start) {

    //alienCollision(Horde);
    for(int i = 0; i < Horde.size(); i++) {
        Horde[i].transform.updateBoundingBox();
        
        wallCollisions(Horde[i], walls);
        laserCollision(Horde[i], lasers, audioObject);
        if(Horde[i].health <= 0) {
            Horde.erase(Horde.begin() + i);
            if(Horde[i].type == HATCHLING) {p1.credits += 20;}
            if(Horde[i].type == SPITTER) {p1.credits += 30;}
            if(Horde[i].type == MATURE) {p1.credits += 50;}
        }

        if(collision(Horde[i].transform, p1.transform) && Horde[i].state != IMPULSE) {
            Horde[i].state = IMPULSE;
            if(Horde[i].type == HATCHLING) {p1.health -= 5;}
            if(Horde[i].type == SPITTER) {p1.health -= 10;}
            if(Horde[i].type == MATURE) {p1.health -= 20;}
            Horde[i].active = false;
        }
        if(Horde[i].state == IMPULSE) {
            heroCollision(Horde[i], p1, dt);

            if(distance(Horde[i].transform.pos, p1.transform.pos) > 150) {
                Horde[i].state = COOL;
                Horde[i].start = getTimeInSeconds();
                Horde[i].active = true;
            }
        }
        
        else if(Horde[i].type == HATCHLING || Horde[i].type == MATURE) {
            if(Horde[i].state == JUMP) {
                jump(Horde[i], Horde[i].currentTarget, dt);
                //printf("is in jumping state\n");
                
                if(distance(Horde[i].transform.pos, Horde[i].currentTarget) < 10) {
                    //printf("has reached target after jump\n");
                    if(Horde[i].type == MATURE) {Horde[i].hitbox = Vec2(80,80);}
                    Horde[i].currentTarget = Vec2(0,0);
                    Horde[i].state = COOL;
                    Horde[i].start = getTimeInSeconds();
                }
            } else if(Horde[i].state == COOL) {
                //printf("cooling off\n");
                float current = getTimeInSeconds();
                Horde[i].elapsed = current - Horde[i].start;
                if(Horde[i].elapsed > Horde[i].cooldown) {
                    //printf("cooling ended, elapsed: %f \n", Horde[i].elapsed);
                    Horde[i].elapsed = 0.0f;
                    Horde[i].state = ANGRY;
                } else {chase(Horde[i], p1, dt);}

                // time setter to wait or chase -- after timer if in range jump again
            } else if(Horde[i].state == ANGRY) {
                chase(Horde[i], p1, dt);

                //printf("is in chase state\n");
                if(distance(Horde[i].transform.pos, p1.transform.pos) < p1.detectionInner) {
                    Horde[i].currentTarget = p1.transform.pos;
                    Horde[i].state = JUMP;
                }
                
            } 
            else if(distance(Horde[i].transform.pos, p1.transform.pos) < p1.detectionOuter) {
                Horde[i].state = ANGRY;
                
            }
        } else {
            if(Horde[i].state == JUMP) {
            jump(Horde[i], Horde[i].currentTarget, dt);

            
                if(distance(Horde[i].transform.pos, Horde[i].currentTarget) > 300 || wallCollisions(Horde[i], walls)) {
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

                if(distance(Horde[i].transform.pos, p1.transform.pos) < p1.detectionInner) {
                    Horde[i].currentTarget = p1.transform.pos;
                    Horde[i].state = JUMP;
                }
                
            } 
            else if(distance(Horde[i].transform.pos, p1.transform.pos) < p1.detectionOuter) {
                Horde[i].state = ANGRY;
                //Horde[i].active = true;
                
            }

            if(Horde[i].shooting) {
                spit(Horde[i], Horde[i].currentTarget, dt);
                // bool collision(Vec2 pos1, float radius1, Vec2 pos2, Vec2 size2)
                if(collision(Horde[i].projectilePos, Horde[i].projectileRad, p1.transform.pos, p1.transform.size)) {
                    printf("hit by acid \n"); printf("Health: %i", p1.health);
                    p1.health -= 25;
                }
            }
        }
        
    }
}

void drawAlien(Alien &alien, bool active, float start) {
    if(active) {
        //printf("inside draw animations \n");
        float current = getTimeInSeconds();
        float elapsed = current - start;
        int frameIndex = (elapsed / alien.animate.duration) * alien.animate.numFrames;
        //printf("Alien animation duration: %f", alien.animate.duration);
        frameIndex %= alien.animate.numFrames;
        drawTexture(alien.animate.frames[frameIndex], alien.transform.pos - alien.transform.size/2, alien.transform.size, alien.transform.angle);
    } else {
        drawTexture(alien.texture, alien.transform.pos - alien.transform.size/2, alien.transform.size, alien.transform.angle);
    }
    
    // made changes to get size getting from transform
    drawRect(alien.transform.pos - alien.transform.size/2, alien.transform.size, Color::red, alien.transform.angle);
    drawRect(alien.transform.getBoundingBox(), Color::green, 0.0f);
    if(alien.shooting) {
        fillCircle(alien.projectilePos, alien.projectileRad, Color::green);
    }
}



// mature ALien -------------------------------------------------------------------------------------------------------------------------------
/*bool addAlien(std::vector<AlienAdult> &Horde, Texture spritesheet) {
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
    alien.transform.size = Vec2(80,80);
    alien.hitbox = Vec2(80, 80);
    alien.state = IDLE;
    alien.active = true;
    alien.texture = subTexture(spritesheet, {0, 0, 60, 60});

    alien.animate.frames.push_back(subTexture(spritesheet, {0, 20, 20, 20}));
    alien.animate.frames.push_back(subTexture(spritesheet, {20, 20, 20, 20}));
    alien.animate.frames.push_back(subTexture(spritesheet, {40, 20, 20, 20}));
    alien.animate.frames.push_back(subTexture(spritesheet, {0, 20, 20, 20}));
    alien.animate.numFrames = 4;
    alien.animate.duration = 0.5;

    Horde.push_back(alien);
    return true;

    // bool hasSpace = true;
    // for(int j = 0; j < Horde.size(); j++) {
    //     if(collision(alien.transform.pos, alien.size, alien.transform.angle, Horde[j].transform.position(), Horde[j].size, Horde[j].transform.angle)) {
    //         hasSpace = false;
    //     }
    // }

    // if(hasSpace) {

    // }
    // // Return result
    // return false;
}

// check if player has entered alien awareness
bool awareOfPlayer(AlienAdult &alien, Hero &p1) {
    if(distance(alien.transform.pos, p1.transform.pos) < p1.detectionOuter) {
        return true;
    } else {
        return false;
    }
}

// chase player
void chase(AlienAdult &alien, Hero &p1, float dt) {  // , float r = 0

    Vec2 toPlayer = unit(p1.transform.pos - alien.transform.pos);


    float targetAngle = atan2(toPlayer.y, toPlayer.x) / M_PI * 180.0f - 270;
    alien.transform.angle = targetAngle;

    alien.vel = toPlayer * 80;
    alien.transform.pos += alien.vel * dt;
}

// jumping
void jump(AlienAdult &alien, Vec2 target, float dt) {
    if(distance(alien.transform.pos, target) < 10) {
        alien.active = true;
        return;
    }
    Vec2 toPlayer = unit(target - alien.transform.pos);
    float targetAngle = atan2(toPlayer.y, toPlayer.x) / M_PI * 180.0f - 270;
    alien.transform.angle = targetAngle;
    alien.hitbox = Vec2(80, 120);


    
    alien.vel = toPlayer * 300;
    alien.transform.pos += alien.vel * dt;
}

// Finite state machine controller - remember state
void fsmAlien(std::vector<AlienAdult> &Horde, Hero &p1, float dt, float start) {

    for(int i = 0; i < Horde.size(); i++) {

        Horde[i].transform.updateBoundingBox();
        if(Horde[i].state == JUMP) {
            jump(Horde[i], Horde[i].currentTarget, dt);
            //printf("is in jumping state\n");
            
            if(distance(Horde[i].transform.pos, Horde[i].currentTarget) < 10) {
                //printf("has reached target after jump\n");
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
            if(distance(Horde[i].transform.pos, p1.transform.pos) < p1.detectionInner) {
                Horde[i].currentTarget = p1.transform.pos;
                Horde[i].state = JUMP;
            }
            
        } 
        else if(distance(Horde[i].transform.pos, p1.transform.pos) < p1.detectionOuter) {
            Horde[i].state = ANGRY;
            
        }

    }
}

void drawAlien(AlienAdult &alien, bool active, float start) {
    if(active) {
        float current = getTimeInSeconds();
        float elapsed = current - start;
        int frameIndex = (elapsed / alien.animate.duration) * alien.animate.numFrames;
        frameIndex %= alien.animate.numFrames;
        drawTexture(alien.animate.frames[frameIndex], alien.transform.pos - alien.size/2, alien.size, alien.transform.angle);
    } else {
        drawTexture(alien.texture, alien.transform.pos - alien.size/2, alien.size, alien.transform.angle);
    }

    drawRect(alien.transform.pos - alien.hitbox/2, alien.hitbox, Color::red, alien.transform.angle);
    
    drawRect(alien.transform.getBoundingBox(), Color::green, 0.0f);

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

    alien.size = Vec2(40, 40);
    alien.transform.size = Vec2(40,40);

    alien.state = IDLE;
    alien.active = true;
    alien.texture = subTexture(spritesheet, {0, 0, 60, 60});

    alien.animate.frames.push_back(subTexture(spritesheet, {0, 40, 20, 20}));
    alien.animate.frames.push_back(subTexture(spritesheet, {20, 40, 20, 20}));
    alien.animate.frames.push_back(subTexture(spritesheet, {40, 40, 20, 20}));
    alien.animate.frames.push_back(subTexture(spritesheet, {0, 40, 20, 20}));
    alien.animate.numFrames = 4;
    alien.animate.duration = 0.5;

    Horde.push_back(alien);
    return true;

    // bool hasSpace = true;
    // for(int j = 0; j < Horde.size(); j++) {
    //     if(collision(alien.transform.pos, alien.size, alien.transform.angle, Horde[j].transform.position(), Horde[j].size, Horde[j].transform.angle)) {
    //         hasSpace = false;
    //     }
    // }

    // if(hasSpace) {

    // }
    // // Return result
    // return false;
}

// chase player
void chase(AlienRanged &alien, Hero &p1, float dt) {  // , float r = 0

    Vec2 toPlayer = unit(p1.transform.pos - alien.transform.pos);


    float targetAngle = atan2(toPlayer.y, toPlayer.x) / M_PI * 180.0f - 270;
    alien.transform.angle = targetAngle;

    alien.vel = toPlayer * 80;
    alien.transform.pos += alien.vel * dt;
}

// jumping
void jump(AlienRanged &alien, Vec2 target, float dt) {
    if(distance(alien.transform.pos, target) < 10) {
        alien.active = true;
        return;
    }
    Vec2 toPlayer = unit(target - alien.transform.pos);
    float targetAngle = atan2(toPlayer.y, toPlayer.x) / M_PI * 180.0f - 270;
    alien.transform.angle = targetAngle;

    alien.vel = toPlayer * -700;
    alien.transform.pos += alien.vel * dt;
}

void spit(AlienRanged &alien, Vec2 target, float dt) {
    Vec2 toPlayer = unit(target - alien.transform.pos);
    alien.projectileVel = toPlayer * 350;
    alien.projectilePos += alien.projectileVel * dt;
}

// Finite state machine controller - remember state
void fsmAlien(std::vector<AlienRanged> &Horde, Hero &p1, float dt, float start) {

    for(int i = 0; i < Horde.size(); i++) {
        Horde[i].transform.updateBoundingBox();
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

            if(distance(Horde[i].transform.pos, p1.transform.pos) < p1.detectionInner) {
                Horde[i].currentTarget = p1.transform.pos;
                Horde[i].state = JUMP;
            }
            
        } 
        else if(distance(Horde[i].transform.pos, p1.transform.pos) < p1.detectionOuter) {
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
        int frameIndex = (elapsed / alien.animate.duration) * alien.animate.numFrames;
        frameIndex %= alien.animate.numFrames;
        drawTexture(alien.animate.frames[frameIndex], alien.transform.pos - alien.size/2, alien.size, alien.transform.angle);
    } else {
        drawTexture(alien.texture, alien.transform.pos - alien.size/2, alien.size, alien.transform.angle);
    }
    drawRect(alien.transform.pos - alien.transform.size/2, alien.transform.size, Color::red, alien.transform.angle);
    drawRect(alien.transform.getBoundingBox(), Color::green, 0.0f);
    if(alien.shooting) {
        fillCircle(alien.projectilePos, alien.projectileRad, Color::green);
    }

}*/
