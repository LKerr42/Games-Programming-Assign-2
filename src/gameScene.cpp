#include <gameScene.h>

#define HUD_PIXEL_SIZE 8

namespace GameScene {
    Font pressStart;

    Hero hero;
    std::vector<Alien> hatchlings;
    std::vector<AlienAdult> matureAliens;
    std::vector<AlienRanged> spitters;

    Texture hatchlingTex;
    Texture matureTex; 
    Texture spitterTex;

    DisplayElement pausedElement;
    DisplayElement letterElement;

    Texture hudBase;
    Rect hudDest;
    Button displayElementExit;
    DisplayElement currentDisplayElement;
    
    int shadows[36][64];

    std::vector<Laser> lasers;
    std::vector<Wall> walls;
    std::list<Item*> levelItems;

    char *currDialogue;
    bool displayingDialogue = false;

    Level currentLevel = LEVEL_ONE;
    DisplayState currentDisplay = HUD;

    Texture background;
    Animation lazerHit;

    std::list<Animation*> playingAnimations;

    const float halfWindowHeight = WINDOW_HEIGHT / 2.0f;
    const int windowWidthShadows = SDL_round(WINDOW_WIDTH/20), windowHeightShadows = SDL_round(WINDOW_HEIGHT/20);
    float current;

    float start;

    void init() {
        setWindowTitle("Pest Control - Playing");
        pressStart = loadFont("assets/fonts/PressStart2P-Regular.ttf");
        background = loadTexture("assets/images/background.png");

        //init hero
        hero.tex = loadTexture("./assets/images/hero.png");

        //init items
        static Texture *baseWeaponTex = new Texture(loadTexture("./assets/images/items/weapon_small_01.png"));
        static Animation *baseWeaponAni = new Animation(loadAnimation("./assets/images/items/weapon_large_animation_grid.png", (Rect){0, 0, 20, 20}, 6, 2.0f, true));

        hero.currWeapon = new Weapon(baseWeaponAni, baseWeaponTex, (Rect){0, 0, 0, 0}, "'E' to grab", 1.0f, 1.0f, 100);
        loadDefinitions();
        loadLevel(currentLevel, levelItems);

        //init other animations
        lazerHit = loadAnimation("./assets/images/laser_hit_animation_grid.png", (Rect){0, 0, 20, 20}, 6, 0.5f, false);
        
        //init Hud
        hudBase = loadTexture("./assets/images/hud_base.png");
        float hudWidth = 128 * 8, hudHeight = 72 * 8;
        hudDest = (Rect){0, 0, hudWidth, hudHeight};

        //init level
        walls.push_back((Wall){(Rect){WINDOW_WIDTH-900.0f, 0, 300, 200}, true, NULL, NULL});
        walls.push_back((Wall){(Rect){WINDOW_WIDTH-600.0f, 0, 300, 200}, true, NULL, NULL});
        walls.push_back((Wall){(Rect){WINDOW_WIDTH-300.0f, 0, 300, 200}, true, NULL, NULL});

        walls.push_back((Wall){(Rect){WINDOW_WIDTH-900.0f, WINDOW_HEIGHT-200.0f, 300, 200}, true, NULL, NULL});
        walls.push_back((Wall){(Rect){WINDOW_WIDTH-600.0f, WINDOW_HEIGHT-200.0f, 300, 200}, true, NULL, NULL});
        walls.push_back((Wall){(Rect){WINDOW_WIDTH-300.0f, WINDOW_HEIGHT-200.0f, 300, 200}, true, NULL, NULL});

        std::cout << "width: " << WINDOW_WIDTH << " height: " << WINDOW_HEIGHT << "\n";

        setupShadows();

        //init display elements
        static Texture pausedTex = loadTexture("./assets/images/paused_splash.png");
        Texture buttonBackground = loadTexture("./assets/images/button_pink.png");

        pausedElement = (DisplayElement){
            "Paused",
            20,
            &pausedTex
        };

        displayElementExit = createButton(
            Vec2(WINDOW_WIDTH - 70, WINDOW_HEIGHT - 40), Vec2(10, 10), "Exit", pressStart, 20.0f, Color::black, Color::white, QUIT, buttonBackground
        );

        //init aliens
        hatchlingTex = loadTexture("./assets/images/aliens/spritesheet.png");
        matureTex = loadTexture("./assets/images/aliens/aliens2.png");
        //spitterTex = loadTexture("./assest/images/aliens/sprit")

        //addAlien(hatchlings, hatchlingTex);
        //hatchlings[0].transform.pos = Vec2(1000, halfWindowHeight);
        addAlien(matureAliens, matureTex);
        matureAliens[0].transform.pos = Vec2(1000, halfWindowHeight);
        //addAlien(spitters, hatchlingTex);
        //spitters[0].transform.pos = Vec2(1000, halfWindowHeight);
        


        current = getTimeInSeconds();
        start = getTimeInSeconds();
    }

    void update(float dt) {
        if (currentDisplay == HUD) {
            //check for display updates
            if (keyPressedThisFrame(KEY_ESCAPE)) {
                currentDisplay = ELEMENT;
                currentDisplayElement = pausedElement;
            }

            //move hero
            if (keyIsPressed(KEY_W)) hero.transform.translate(Vec2(0, -3));
            if (keyIsPressed(KEY_A)) hero.transform.translate(Vec2(-3, 0));
            if (keyIsPressed(KEY_S)) hero.transform.translate(Vec2(0, 3));
            if (keyIsPressed(KEY_D)) hero.transform.translate(Vec2(3, 0));

            hero.transform.rotateTo(mousePosition());
            hero.transform.updateBoundingBox();

            //Detect collision, and then calculate overlap to push back hero
            for (Wall W : walls) {
                if (collision(hero.transform.getBoundingBox(), 0.0f, W.rect, 0.0f)) {
                    handleCollision(W.rect);
                }
            }

            //check out of bounds
            Rect box = hero.transform.getBoundingBox();
            if (box.x < 0) hero.transform.translate(Vec2(-box.x, 0));

            if (box.y < 0) hero.transform.translate(Vec2(0, -box.y));

            if (box.x + box.width > WINDOW_WIDTH) {
                hero.transform.translate(Vec2(WINDOW_WIDTH - (box.x + box.width), 0));
            }

            if (box.y + box.height > WINDOW_HEIGHT) {
                hero.transform.translate(Vec2(0, WINDOW_HEIGHT - (box.y + box.height)));
            }

            //update lasers
            if (keyIsPressed(KEY_R)) {
                hero.currWeapon->reloadTimer->active = true;
            }

            if (updateTimer(hero.currWeapon->reloadTimer, dt)) {
                hero.currWeapon->currEnergy = hero.currWeapon->fullEnergy;
                hero.currWeapon->reloadTimer->active = false;
            }

            bool canFire = updateTimer(hero.currWeapon->fireTimer, dt);
            if (mouseButtonIsPressed(MOUSE_BUTTON_LEFT) 
                    && hero.currWeapon->currEnergy > 0 
                    && canFire) {
                lasers.emplace_back();
                hero.currWeapon->currEnergy -= 10;
                resetTimer(hero.currWeapon->fireTimer);
            }

            for (int i = 0; i < lasers.size();) {
                Laser &L = lasers.at(i);

                L.transform.translateByAngle(L.speed * dt);

                if (L.transform.getPosition().x > WINDOW_WIDTH || L.transform.getPosition().x < 0 ||
                        L.transform.getPosition().y > WINDOW_HEIGHT || L.transform.getPosition().y < 0) {
                    deleteLazer(L, i);
                    continue;
                }

                //check walls against lasers
                //time complexity is O(n^2), but with very little walls and lasers realistically, this is pretty much free
                for (Wall& w : walls) {
                    Rect laserRect = {L.transform.getPosition().x, L.transform.getPosition().y, L.transform.getSize().x, L.transform.getSize().y};
                    if (collision(laserRect, L.transform.getAngle(), w.rect, 0.0f)) {
                        deleteLazer(L, i);
                        continue;
                    }
                }
                
                i++;
            }

            //check contact with items
            int ammountColls = 0;
            for (auto item = levelItems.begin(); item != levelItems.end(); ) {
                if (collision(hero.transform.getPosition(LOCAL), hero.sightRad, 
                        Vec2((*item)->dst.x, (*item)->dst.y), Vec2((*item)->dst.width, (*item)->dst.height))) {
                    currDialogue = (*item)->hoverDialogue;
                    displayingDialogue = true;
                    ammountColls++;

                    if (keyPressedThisFrame(KEY_E)){
                        Item *dropped = (*item)->dropItem(hero);
                        if (dropped != nullptr) levelItems.push_back(dropped);
                        (*item)->pickup(hero, &currentDisplay, &currentDisplayElement);
                        item = levelItems.erase(item);
                        break;
                    } else {
                        ++item;
                    }
                } else {
                    ++item;
                }
            }
            if (ammountColls == 0) displayingDialogue = false;

            //if (distance(hero.transform.getPosition(LOCAL), Vec2(item->dst.x, item->dst.y)) < hero.sightRad) {}

            //update aliens
            //fsmAlien(hatchlings, hero, dt, current); // 4th parameter may be a dud
            fsmAlien(matureAliens, hero, dt, current);
            //fsmAlien(spitters, hero, dt, current);

            //check updates to shadows
            Vec2 tip = hero.transform.getPosition(LOCAL);
            float angleRad = hero.transform.angle * M_PI / 180.0f;
            float length = hero.sightLength;
            float spread = 30.0f;

            float leftRad  = angleRad - spread * M_PI / 180.0f;
            float rightRad = angleRad + spread * M_PI / 180.0f;

            hero.sightLeft = Vec2(
                tip.x + cos(leftRad) * length,
                tip.y + sin(leftRad) * length
            );

            hero.sightRight = Vec2(
                tip.x + cos(rightRad) * length,
                tip.y + sin(rightRad) * length
            );

            int sightRadShadows = SDL_round(hero.sightRad / 20);
            Vec2 HeroPosShadows = Vec2(SDL_round(hero.transform.getPosition().x / 20), SDL_round(hero.transform.getPosition().y / 20));

            for (int i = HeroPosShadows.y - sightRadShadows; i < HeroPosShadows.y + sightRadShadows; i++) {
                for (int j = HeroPosShadows.x - sightRadShadows; j < HeroPosShadows.x + sightRadShadows + 2; j++) {
                    if (i < 0 || j < 0 || i >= windowHeightShadows || j >= windowWidthShadows) {
                        continue;
                    }
                    if (collision(hero.transform.getPosition(LOCAL), hero.sightRad, 
                            Vec2(j*20, i*20), Vec2(20, 20))) {
                        shadows[i][j] = 0;
                    }
                }
            }

        } else {
            //check for display updates
            if (keyPressedThisFrame(KEY_ESCAPE)) currentDisplay = HUD;

            //run button
            if(collision(mousePosition(), displayElementExit.position, displayElementExit.size)) {
                displayElementExit.hovered = true;

                // Mouse Click
                if(mouseButtonPressedThisFrame(MOUSE_BUTTON_LEFT)) {
                    currentDisplay = HUD;
                }
            } else {
                displayElementExit.hovered = false;
            }
        }
    }

    void render(float lag) {
        //clear((Color){50, 50, 50, 255});

        //drawText(Vec2(10, 10), "Gaming time", (Color){219, 0, 172, 255}, pressStart, 24);

        current = getTimeInSeconds();

        drawTexture(background, Vec2(0, 0), Vec2(WINDOW_WIDTH, WINDOW_HEIGHT));

        //hero
        drawTexture(hero.tex, hero.transform.getPosition(), hero.transform.getSize(), hero.transform.getAngle());

        if (displayingDialogue) {
            displayDialogue(currDialogue);
        }

        //lasers
        for (Laser &L : lasers) {
            //drawTexture(lazer, L.transform.getPosition(), L.transform.getSize(), L.transform.getAngle());
            fillRect(L.transform.getPosition(), L.transform.getSize(), Color::green, L.transform.getAngle());
        }

        //walls
        for (Wall &W : walls) {
            drawRect(W.rect, Color::yellow);
        }

        //display items
        for (Item* item : levelItems) {
            int indx = getAnimationIndex(item->animationLarge, current, nullptr);
            drawTexture(item->animationLarge->frames.at(indx), item->dst);
        }

        //display current animations
        for (auto it = playingAnimations.begin(); it != playingAnimations.end();) {
            bool isOver = false;
            int indx = getAnimationIndex((*it), current, &isOver);
            drawTexture((*it)->frames.at(indx), (Rect){(*it)->dst.x, (*it)->dst.y, 20, 20}); 

            if (isOver) {
                it = playingAnimations.erase(it);
            } else {
                ++it;
            }
        }

        for (Alien a : hatchlings) {
            drawAlien(a, a.active, start);
        }
        for (AlienAdult a : matureAliens) {
            drawAlien(a, a.active, start);
        }
        for (AlienRanged a : spitters) {
            drawAlien(a, a.active, start);
        }

        //Shadows
        /*for (int i = 0; i < 36; i++) {
            for (int j = 0; j < 64; j++) {
                Color currCol;
                if (shadows[i][j] != 0) {
                    if (shadows[i][j] == 1) {
                        currCol = (Color){37, 37, 75};
                    } else if (shadows[i][j] == 2) {
                        currCol = (Color){25, 25, 50};
                    } else if (shadows[i][j] == 3) {
                        currCol = (Color){12, 12, 25};
                    } else if (shadows[i][j] == 4) {
                        currCol = (Color){7, 7, 15};
                    } else {
                        currCol = (Color){2, 2, 5};
                    }
                    fillRect(Vec2(j*20, i*20), Vec2(20, 20), currCol);
                }
            }
        }*/

        //debug 
        // -- TODO: remove before submition --
        drawCircle(hero.transform.getPosition(LOCAL), hero.sightRad, (Color){255, 128, 0, 255});
        drawRect(hero.transform.getBoundingBox(), Color::red, 0.0f);
        drawRect(hero.transform.getPosition(), hero.transform.getSize(), Color::green, hero.transform.getAngle());

        //debug detection radius
        drawCircle(hero.transform.getPosition(LOCAL), hero.detectionInner, Color::red);
        drawCircle(hero.transform.getPosition(LOCAL), hero.detectionOuter, Color::green);

        //create triangle
        drawLine(hero.transform.getPosition(LOCAL), hero.sightLeft, Color::yellow);
        drawLine(hero.transform.getPosition(LOCAL), hero.sightRight, Color::yellow);
        drawLine(hero.sightLeft, hero.sightRight, Color::yellow);

        //overlay display
        if (currentDisplay == HUD) {
            drawTexture(hudBase, hudDest);

            //health bar
            Color healthCol = (hero.health > 50) ? Color::green : Color::red;
            float healthBarWidth = 256 * (hero.health / 100);
            fillRect(Vec2(22*HUD_PIXEL_SIZE, 5*HUD_PIXEL_SIZE), Vec2(healthBarWidth, 24), healthCol);

            //energy bar
            float energyPercentage = (hero.currWeapon->currEnergy / hero.currWeapon->fullEnergy);
            float energyBarWidth = 192 * energyPercentage;
            fillRect(Vec2(20*HUD_PIXEL_SIZE, 12*HUD_PIXEL_SIZE), Vec2(energyBarWidth, 16), (Color){66, 135, 245, 255});

            //inventory
            if (hero.currWeapon != nullptr) drawTexture(*hero.currWeapon->smallTexture, (Rect){24, 24*HUD_PIXEL_SIZE, 40, 40});
            if (hero.currArmour != nullptr) drawTexture(*hero.currArmour->smallTexture, (Rect){24, 32*HUD_PIXEL_SIZE, 40, 40});
            if (hero.currUpgrade != nullptr) drawTexture(*hero.currUpgrade->smallTexture, (Rect){24, 40*HUD_PIXEL_SIZE, 40, 40});
        } else {
            float sizeX, sizeY;
            SDL_GetTextureSize(currentDisplayElement.mainTexture->texture, &sizeX, &sizeY);
            Vec2 displayTexturesize = Vec2(sizeX, sizeY);
            Vec2 displayTexturePos = Vec2(
                WINDOW_WIDTH/2 - sizeX/2,
                WINDOW_HEIGHT/2 - sizeY/2
            );

            fillRect(Vec2(0, 0), Vec2(WINDOW_WIDTH, WINDOW_HEIGHT), 0, 0, 0, 128U, 0.0f);
            drawText(Vec2(0, 0), currentDisplayElement.dialogue, Color::white, pressStart, currentDisplayElement.fontSize);
            drawTexture(*currentDisplayElement.mainTexture, displayTexturePos, displayTexturesize);

            renderButton(displayElementExit);
        }
    }

    void close() {

    }

    void displayDialogue(const char *msg) {
        Vec2 heroPos = hero.transform.getPosition(LOCAL);
        Vec2 msgSize = measureText(msg, pressStart, 14);
        Vec2 pos = Vec2(heroPos.x - (msgSize.x/2), heroPos.y - 30);
        drawText(pos, msg, Color::white, pressStart, 14, 0.0f);
    }

    void setupShadows() {
        for (int i = 0; i < 36; i++) {
            for (int j = 0; j < 64; j++) {
                shadows[i][j] = SDL_rand(5) + 1;
            }
        }

        smoothShadows();
    }

    void smoothShadows() {
        int temp[36][64];

        for (int y = 1; y < 35; y++) {
            for (int x = 1; x < 63; x++) {
                int total = 0;
                int count = 0;

                for (int oy = -1; oy <= 1; oy++) {
                    for (int ox = -1; ox <= 1; ox++) {
                        total += shadows[y + oy][x + ox];
                        count++;
                    }
                }

                temp[y][x] = total / count;
            }
        }

        for (int y = 1; y < 35; y++) {
            for (int x = 1; x < 63; x++) {
                shadows[y][x] = temp[y][x];
            }
        }
    }

    void handleCollision(Rect collidingWith) {
        Rect heroBox = hero.transform.getBoundingBox();
        float left = (heroBox.x + heroBox.width) - collidingWith.x;
        float right = (collidingWith.x +  collidingWith.width) - heroBox.x;
        float top = (heroBox.y + heroBox.height) - collidingWith.y;
        float bottom = (collidingWith.y +  collidingWith.height) - heroBox.y;

        float minOverlap = min(min(left, right), min(top, bottom));

        if (minOverlap == left) {
            hero.transform.translate(Vec2(-left, 0));
        } else if (minOverlap == right) {
            hero.transform.translate(Vec2(right, 0));
        } else if (minOverlap == top) {
            hero.transform.translate(Vec2(0, -top));
        } else if (minOverlap == bottom) {
            hero.transform.translate(Vec2(0, bottom));
        }
    }

    Laser::Laser() {
        transform = Transform(hero.transform.getPosition(LOCAL), hero.transform.getAngle(), Vec2(13, 3));
        speed = 300.0f;
    };

    void deleteLazer(Laser& L, int indx) {
        Animation* newLazer = (Animation*)calloc(1, sizeof(Animation));
        *newLazer = lazerHit;

        newLazer->dst = L.transform.getPosition();
        newLazer->start = getTimeInSeconds();
        playingAnimations.push_back(newLazer);
        
        lasers.erase(lasers.begin() + indx);      
    }
}