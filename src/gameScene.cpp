#include <gameScene.h>

#define HUD_PIXEL_SIZE 8

namespace GameScene {
    Font pressStart;

    audioClips soundFX;

    Hero hero;
    std::vector<Alien> aliens;

    Timer waveCooldown;
    int waveCounter;
    int numH, numM, numS;

    std::string cooldownStr;
    bool displayCountdown;
    bool gameIsActive = false;

    Texture alienSpritesheet;

    DisplayElement pausedElement;
    DisplayElement letterElement;

    Texture hudBase;
    Rect hudDest;
    Button displayElementExit;
    DisplayElement currentDisplayElement;
    Texture workbenchBackground;
    
    int shadows[36][64];
    Texture wallTexture;

    upgradePurchase products[6];

    std::vector<Laser> lasers;
    std::vector<Wall> walls;
    
    std::list<Element*> elements;

    char *currDialogue;
    bool displayingDialogue = false;

    DisplayState currentDisplay = HUD;

    Texture background;
    Animation lazerHit;

    std::list<Animation*> playingAnimations;

    const float halfWindowHeight = WINDOW_HEIGHT / 2.0f;
    const int windowWidthShadows = SDL_round(WINDOW_WIDTH/20), windowHeightShadows = SDL_round(WINDOW_HEIGHT/20);
    float current;

    float start;
    std::vector<Vec2> wallInput;
    std::vector<Transform> laserInput;

    bool playAliens = false;

    void init() {
        setWindowTitle("Pest Control - Playing");
        pressStart = loadFont("assets/fonts/PressStart2P-Regular.ttf");
        background = loadTexture("assets/images/backgrounG.png");

        loadLaserShoot(soundFX, "./assets/Soundfx/laserSound/freesound_community-laser-45816.mp3");
        loadLaserHit(soundFX, "./assets/Soundfx/laserSound/universfield-blaster-shot-229313.mp3");
        loadAlienSounds(soundFX, "./assets/Soundfx/laserSound/freesound_community-alien-xenomorph-hissing-and-growling-with-and-without-fx-22773.mp3");
        loadHurtSound(soundFX, "./assets/Soundfx/laserSound/homemade_sfx-slap-hurt-pain-sound-effect-262618.mp3");


        //init hero
        hero.tex = loadTexture("./assets/images/hero.png");
        hero.health = 100;
        std::cout << hero.health << "\n";

        hero.currWeapon = new Weapon(
            new Texture(loadTexture("./assets/images/items/weapon_small.png")),
            0.5f, 2.0f, 100
        ); 

        hero.currArmour = new Armour(
            new Texture(loadTexture("./assets/images/items/armour_small.png")),
            5, 100
        ); 
        hero.currUpgrade = new Upgrade(
            new Texture(loadTexture("./assets/images/items/upgrade_small.png")),
            NONE, 0.0f, 0
        );

        //init elements
        static DisplayElement messageElement = (DisplayElement){
            "Press 'G' to start defending", 
            32,
            new Texture(loadTexture("./assets/images/message_large.png"))
        };

        elements.emplace_back(new Element(
            new Animation(loadAnimation("./assets/images/items/message_animation_grid.png", (Rect){0, 0, 20, 20}, 6, 2.0f, true)), 
            (Rect){100, WINDOW_HEIGHT-40, 40, 40}, "'E' to read", SHOW_DISPLAY, &messageElement
        ));

        elements.emplace_back(new Element(
            new Animation(loadAnimation("./assets/images/items/workbench_animation_grid.png", (Rect){0, 0, 30, 20}, 3, 3.0f, true)), 
            (Rect){500, WINDOW_HEIGHT-40, 60, 40}, "'E' to open", OPEN_WORKBENCH, nullptr
        ));

        elements.emplace_back(new Element(
            new Animation(loadAnimation("./assets/images/items/heal_station.png", (Rect){0, 0, 30, 20}, 3, 3.0f, true)), 
            (Rect){700, WINDOW_HEIGHT-40, 60, 40}, "'E' to heal (100c)", HEAL_HERO, nullptr
        ));

        wallTexture = loadTexture("./assets/images/wall.png");

        //init other animations
        lazerHit = loadAnimation("./assets/images/laser_hit_animation_grid.png", (Rect){0, 0, 20, 20}, 6, 0.5f, false);
        
        //init Hud
        hudBase = loadTexture("./assets/images/hud_base_v2.png");
        float hudWidth = 128 * 8, hudHeight = 72 * 8;
        hudDest = (Rect){0, 0, hudWidth, hudHeight};

        //init level
        walls.emplace_back(0, 0);
        walls.emplace_back(WINDOW_WIDTH-200, 0);
        walls.emplace_back(0, WINDOW_HEIGHT-200);
        walls.emplace_back(WINDOW_WIDTH-200, WINDOW_HEIGHT-200);

        std::cout << "width: " << WINDOW_WIDTH << " height: " << WINDOW_HEIGHT << "\n";

        setupShadows();

        //init display elements
        static Texture pausedTex = loadTexture("./assets/images/paused_splash.png");
        Texture buttonBackground = loadTexture("./assets/images/button_pink.png");
        workbenchBackground = loadTexture("./assets/images/workbench_background.png");

        pausedElement = (DisplayElement){
            "Paused",
            20,
            &pausedTex
        };

        displayElementExit = createButton(
            Vec2(WINDOW_WIDTH - 70, WINDOW_HEIGHT - 40), Vec2(10, 10), "Exit", pressStart, 20.0f, Color::black, Color::white, QUIT, buttonBackground
        );

        //init products
        products[0] = (upgradePurchase){0, 100, 5, "total energy"};
        products[1] = (upgradePurchase){1, 100, 0.1, "firing speed"};
        products[2] = (upgradePurchase){2, 100, 0.1, "reload speed"};
        products[3] = (upgradePurchase){3, 100, 5, "resistance"};
        products[4] = (upgradePurchase){4, 100, 5, "health"};
        products[5] = (upgradePurchase){5, 100, 1, "speed"};

        //init aliens
        alienSpritesheet = loadTexture("./assets/images/aliens/alien_spritesheet_v3.png");  
        
        //init waves
        activateTimer(&waveCooldown, 10.0f, false, false);
        displayCountdown = false;
        cooldownStr = "10";

        current = getTimeInSeconds();
        start = getTimeInSeconds();

        std::cout << hero.health << "\n";
    }

    void update(float dt) {
        if (hero.health <= 0) {
            currentDisplay = DEATH;
        }

        if(!aliens.empty() && playAliens == false) {
            playAlienSounds(soundFX);
            playAliens = true;
        }
        if(aliens.empty()) {
            playAliens = false;
            stopAlienSounds(soundFX);
        }

        //playAudio(soundFX.AlienSounds, 0.5f, true);
        if (currentDisplay == HUD) {
            //check for display updates
            if (keyPressedThisFrame(KEY_ESCAPE)) {
                currentDisplay = ELEMENT;
                currentDisplayElement = pausedElement;
            }

            //check for wave updates
            if (aliens.empty() && gameIsActive) { //countdown once the horde is empty
                waveCooldown.active = true;

                if (updateTimer(&waveCooldown, dt)) {   
                    waveCooldown.active = false;
                    displayCountdown = false;
                    waveCounter++;

                    //addAlien(aliens, alienSpritesheet, HATCHLING);
                    numH++;
                    if (numH == 6) {
                        numH = 1;
                        numM++;
                    }
                    if (numM == 6) {
                        numM = 1;
                        numS++;
                    }

                    addAliensForWave(numH, numM, numS, waveCounter);
                    //std::cout << "Spawn more aliens\n";
                } else if (waveCooldown.active) {
                    cooldownStr = std::to_string((int)(10 - SDL_floor(waveCooldown.elasped)));
                    displayCountdown = true;
                }
                
            }

            //debug
            // -- TODO: REMOVE BEFORE SUBMITION PLEASE FOR THE LOVE OF GOD --
            if (keyIsPressed(KEY_K)) {
                aliens.clear();
            }
            if (keyIsPressed(KEY_G)) {
                gameIsActive = true;
            }

            //move hero
            if (keyIsPressed(KEY_W)) hero.transform.translate(Vec2(0, -hero.speed));
            if (keyIsPressed(KEY_A)) hero.transform.translate(Vec2(-hero.speed, 0));
            if (keyIsPressed(KEY_S)) hero.transform.translate(Vec2(0, hero.speed));
            if (keyIsPressed(KEY_D)) hero.transform.translate(Vec2(hero.speed, 0));

            hero.transform.rotateTo(mousePosition());
            hero.transform.updateBoundingBox();

           //Detect collision, and then calculate overlap to push back hero
            for (Wall W : walls) {
                if (distance(W.pos, hero.transform.pos) > hero.detectionInner) {
                    continue;
                }
                Rect wallRect = {W.pos.x, W.pos.y, W.size.x, W.size.y};
                if (collision(hero.transform.getBoundingBox(), 0.0f, wallRect, 0.0f)) {
                    handleCollision(wallRect);
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
                playLaserShoot(soundFX);
            }

            for (int i = 0; i < lasers.size();) {
                Laser &L = lasers.at(i);

                L.transform.translateByAngle(L.speed * dt);

                if (L.transform.getPosition().x >= WINDOW_WIDTH-20 || L.transform.getPosition().x < 0 ||
                        L.transform.getPosition().y >= WINDOW_HEIGHT-20 || L.transform.getPosition().y < 0) {
                    deleteLazer(L, i);
                    continue;
                }

                //check walls against lasers
                //time complexity is O(n^2), but with very little walls and lasers realistically, this is pretty much free
                for (Wall& w : walls) {
                    Rect laserRect = {L.transform.getPosition().x, L.transform.getPosition().y, L.transform.getSize().x, L.transform.getSize().y};
                    Rect wallRect = {w.pos.x, w.pos.y, w.size.x, w.size.y};
                    if (collision(laserRect, L.transform.getAngle(), wallRect, 0.0f)) {
                        deleteLazer(L, i);
                        continue;
                    }
                }

                i++;
            }

            //check contact with elements
            int collisionCount = 0;
            for (auto ele = elements.begin(); ele != elements.end(); ++ele) {
                if (collision(hero.transform.getPosition(LOCAL), hero.sightRad, 
                        Vec2((*ele)->dst.x, (*ele)->dst.y), Vec2((*ele)->dst.width, (*ele)->dst.height))) {
                    currDialogue = (*ele)->hoverDialogue;
                    displayingDialogue = true;
                    collisionCount++;

                    if (keyPressedThisFrame(KEY_E)) {
                        (*ele)->pickup(hero, currentDisplay, currentDisplayElement);
                    }
                } 
            }
            if (collisionCount == 0) displayingDialogue = false;

            //update aliens
            wallInput.clear();
            for(Wall l : walls) {wallInput.push_back(l.pos);}
            fsmAlien(aliens, wallInput, lasers, hero, soundFX, dt, current);
        } else if (currentDisplay == ELEMENT) {
            //check for display updates
            if (keyPressedThisFrame(KEY_ESCAPE)) currentDisplay = HUD;

            runButton(displayElementExit);
        } else if (currentDisplay == WORKBENCH) {
            //check for display updates
            if (keyPressedThisFrame(KEY_ESCAPE)) currentDisplay = HUD;

            runButton(displayElementExit);

            //get user input for upgrade

            if (keyReleasedThisFrame(KEY_1)) { //more energy
                attemptPurchase(products[0], hero.currWeapon->fullEnergy, products[0].value);
            } else if (keyReleasedThisFrame(KEY_2)) { //fire speed
                attemptPurchase(products[1], hero.currWeapon->fireTimer->interval, -products[1].value);
            } else if (keyReleasedThisFrame(KEY_3)) { //reload speed
                attemptPurchase(products[2], hero.currWeapon->reloadTimer->interval, -products[2].value);
            } else if (keyReleasedThisFrame(KEY_4)) { //resistance
                attemptPurchase(products[3], hero.currArmour->resistance, products[3].value);
            } else if (keyReleasedThisFrame(KEY_5)) { //health
                attemptPurchase(products[4], hero.currArmour->healthUpgrade, products[4].value);
            } else if (keyReleasedThisFrame(KEY_6)) { //upgrade
                attemptPurchase(products[5], hero.speed, products[5].value);
            } 
        } else {
            //check for display updates
            if (keyPressedThisFrame(KEY_RETURN)) {
                hero.health = 100;
                waveCounter = 0;
                aliens.clear();
                hero.transform.pos = Vec2(150, WINDOW_HEIGHT/2.0f);
                hero.currWeapon->currEnergy = hero.currWeapon->fullEnergy;
                currentDisplay = HUD;
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
            drawTexture(wallTexture, W.pos, W.size);
            //drawRect(W.rect, Color::yellow);
        }

        //display elements
        for (Element* ele : elements) {
            int indx = getAnimationIndex(ele->animate, current, nullptr);
            drawTexture(ele->animate->frames.at(indx), ele->dst);
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

        for (Alien a : aliens) {
            drawAlien(a, a.active, start);
        }

        if (displayCountdown) {
            Color col = ((int)(10 - SDL_floor(waveCooldown.elasped)) > 5) ? Color::white : Color::red;
            drawText(Vec2(WINDOW_WIDTH/2, WINDOW_HEIGHT/2), cooldownStr.c_str(), col, pressStart, 32, 0.0f);
        }

        //pre-hud
        std::string creditStr = "Credits: " + std::to_string(hero.credits);
        drawText(Vec2(1050, 0), creditStr.c_str(), Color::yellow, pressStart, 16);

        std::string waveStr = "Wave: " + std::to_string(waveCounter);
        drawText(Vec2(1050, 30), waveStr.c_str(), Color::white, pressStart, 16);

        //overlay display
        if (currentDisplay == HUD) {
            //health bar
            fillRect(Vec2(22*HUD_PIXEL_SIZE, 5*HUD_PIXEL_SIZE), Vec2(256, 24), Color::red);
            //Color healthCol = (hero.health > 50) ? Color::green : Color::red;
            
            float healthBarWidth = 256.0f * (hero.health / 100.0f);
            //std::cout << healthBarWidth << " ";
            fillRect(Vec2(22*HUD_PIXEL_SIZE, 5*HUD_PIXEL_SIZE), Vec2(healthBarWidth, 24), Color::green);

            drawTexture(hudBase, hudDest);

            //energy bar
            float energyPercentage = (hero.currWeapon->currEnergy / hero.currWeapon->fullEnergy);
            float energyBarWidth = 192 * energyPercentage;
            fillRect(Vec2(20*HUD_PIXEL_SIZE, 12*HUD_PIXEL_SIZE), Vec2(energyBarWidth, 16), (Color){66, 135, 245, 255});
            
            //inventory
            if (hero.currWeapon != nullptr) drawTexture(*hero.currWeapon->smallTexture, (Rect){24, 24*HUD_PIXEL_SIZE, 40, 40});
            if (hero.currArmour != nullptr) drawTexture(*hero.currArmour->smallTexture, (Rect){24, 32*HUD_PIXEL_SIZE, 40, 40});
            if (hero.currUpgrade != nullptr) drawTexture(*hero.currUpgrade->smallTexture, (Rect){24, 40*HUD_PIXEL_SIZE, 40, 40});
        } else if (currentDisplay == ELEMENT) {
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
        } else if (currentDisplay == WORKBENCH) {
            float sizeX, sizeY;
            SDL_GetTextureSize(workbenchBackground.texture, &sizeX, &sizeY);
            sizeX *= 5;
            sizeY *= 5;
            Vec2 displayTexturesize = Vec2(sizeX, sizeY);
            Vec2 displayTexturePos = Vec2(
                WINDOW_WIDTH/2 - sizeX/2,
                WINDOW_HEIGHT/2 - sizeY/2
            );
            Vec2 topLeftCorner = displayTexturePos + Vec2(30,30);

            fillRect(Vec2(0, 0), Vec2(WINDOW_WIDTH, WINDOW_HEIGHT), 0, 0, 0, 128U, 0.0f);
            drawText(Vec2(0, 0), "Workbench, enter no. to upgrade", Color::white, pressStart, 32);
            drawTexture(workbenchBackground, displayTexturePos, displayTexturesize);

            //strings
            std::string purchaseInfo[6];
            for (int i = 0; i < 6; i++) {
                int visibleVal = (products[i].value < 1) ? 1 : products[i].value;
                purchaseInfo[i] = "(" + std::to_string(i+1) + "): " + std::to_string(products[i].cost) + "c ... " + "+" + std::to_string(visibleVal) + " " + products[i].title;
            }

            //Weapon
            drawText(topLeftCorner + Vec2(0, 0), "Weapon:-", Color::green, pressStart, 32);
            drawText(topLeftCorner + Vec2(0, 50), purchaseInfo[0].c_str(), Color::white, pressStart, 32);
            drawText(topLeftCorner + Vec2(0, 100), purchaseInfo[1].c_str(), Color::white, pressStart, 32);
            drawText(topLeftCorner + Vec2(0, 150), purchaseInfo[2].c_str(), Color::white, pressStart, 32);

            //Armour
            drawText(topLeftCorner + Vec2(0, 200), "Armour:-", Color::green, pressStart, 32);
            drawText(topLeftCorner + Vec2(0, 250), purchaseInfo[3].c_str(), Color::white, pressStart, 32);
            drawText(topLeftCorner + Vec2(0, 300), purchaseInfo[4].c_str(), Color::white, pressStart, 32);

            //upgrade
            drawText(topLeftCorner + Vec2(0, 350), "Upgrade:-", Color::green, pressStart, 32);
            drawText(topLeftCorner + Vec2(0, 400), purchaseInfo[5].c_str(), Color::white, pressStart, 32);

            renderButton(displayElementExit);
        } else {
            fillRect(Vec2(0, 0), Vec2(WINDOW_WIDTH, WINDOW_HEIGHT), 0, 0, 0, 128U, 0.0f);

            drawText(Vec2(0, 0), "You Died! Press enter to resume", Color::white, pressStart, 32);
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

    void initWalls() {
        char* levelOne[36] = {};
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

    void runButton(Button &b) {
        if(collision(mousePosition(), b.position, b.size)) {
            b.hovered = true;

            // Mouse Click
            if(mouseButtonPressedThisFrame(MOUSE_BUTTON_LEFT)) {
                currentDisplay = HUD;
            }
        } else {
            b.hovered = false;
        }
    }

    bool attemptPurchase(upgradePurchase& purchase, float& data, float updateVal) {
        if (hero.credits < purchase.cost) {
            return false;
        }

        //remove credits
        hero.credits =- purchase.cost;

        //update data
        data += updateVal;
        if (purchase.indx == 3 && data > 25) {
            data = 25;
        }
        
        return true;
    }

    void addAliensForWave(int numHatching, int numMature, int numSpitter, int waveNum) {
        for (int i = 0; i < numHatching; i++) {
            addAlien(aliens, alienSpritesheet, HATCHLING);
        }

        for (int i = 0; i < numMature; i++) {
            addAlien(aliens, alienSpritesheet, MATURE);
        }

        for (int i = 0; i < numSpitter; i++) {
            addAlien(aliens, alienSpritesheet, SPITTER);
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

    bool pointInTriangle(Vec2 p, Vec2 a, Vec2 b, Vec2 c) {
        float d1 = (p.x - b.x)*(a.y - b.y) - (a.x - b.x)*(p.y - b.y);
        float d2 = (p.x - c.x)*(b.y - c.y) - (b.x - c.x)*(p.y - c.y);
        float d3 = (p.x - a.x)*(c.y - a.y) - (c.x - a.x)*(p.y - a.y);

        bool hasNeg = (d1 < 0) || (d2 < 0) || (d3 < 0);
        bool hasPos = (d1 > 0) || (d2 > 0) || (d3 > 0);

        return !(hasNeg && hasPos);
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