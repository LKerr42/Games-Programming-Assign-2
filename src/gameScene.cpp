#include <gameScene.h>

#define HUD_PIXEL_SIZE 8

namespace GameScene {
    Font pressStart;
    Texture hudBase;
    Rect hudDest;

    Hero hero;

    DisplayElement pausedElement;
    Button displayElementExit;

    DisplayElement currentDisplayElement;

    std::vector<Laser> lasers;
    std::vector<Wall> walls;
    std::list<Item*> levelItems;

    char *currDialogue;
    bool displayingDialogue = false;

    Level currentLevel = LEVEL_ONE;
    DisplayState currentDisplay = HUD;
    Texture smallWeapons[3], smallArmour[3], smallUpgrades[3], background;
    Animation armourAnimation;
    Animation weaponAnimation;

    const float halfWindowHeight = WINDOW_HEIGHT / 2.0f;

    void init() {
        setWindowTitle("Pest Control - Playing");
        pressStart = loadFont("assets/fonts/PressStart2P-Regular.ttf");
        background = loadTexture("assets/images/background.png");

        //init hero
        hero.tex = loadTexture("./assets/images/hero.png");

        //init items
        openItemImages();
        
        hero.currWeapon = new Weapon(&weaponAnimation, &smallWeapons[0], (Rect){0, 0, 0, 0}, 1.0f, 1.0f, 100);

        levelItems.push_back(new Armour(&armourAnimation, &smallArmour[0], (Rect){640, halfWindowHeight, 0, 0}, 100, 100));
        levelItems.push_back(new Weapon(&weaponAnimation, &smallWeapons[0], (Rect){700, halfWindowHeight, 0, 0}, 0.25f, 2.0f, 100));

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
                    Rect heroBox = hero.transform.getBoundingBox();
                    float left = (heroBox.x + heroBox.width) - W.rect.x;
                    float right = (W.rect.x +  W.rect.width) - heroBox.x;
                    float top = (heroBox.y + heroBox.height) - W.rect.y;
                    float bottom = (W.rect.y +  W.rect.height) - heroBox.y;

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
                    lasers.erase(lasers.begin() + i);
                } else {
                    i++;
                }
            }

            //check contact with items
            int ammountColls = 0;
            for (auto item = levelItems.begin(); item != levelItems.end(); ) {
                if (collision(hero.transform.getPosition(LOCAL), hero.sightRad, 
                        Vec2((*item)->dst.x, (*item)->dst.y), Vec2((*item)->dst.width, (*item)->dst.height))) {
                    currDialogue = "'E' to grab";
                    displayingDialogue = true;
                    ammountColls++;

                    if (keyPressedThisFrame(KEY_E)){
                        Item *dropped = (*item)->dropItem(hero);
                        if (dropped != nullptr) levelItems.push_back(dropped);
                        (*item)->pickup(hero);
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

        float current = getTimeInSeconds();

        drawTexture(background, Vec2(0, 0), Vec2(WINDOW_WIDTH, WINDOW_HEIGHT));

        //hero
        drawCircle(hero.transform.getPosition(LOCAL), hero.sightRad, (Color){255, 128, 0, 255});
        drawRect(hero.transform.getBoundingBox(), Color::red, 0.0f);
        drawRect(hero.transform.getPosition(), hero.transform.getSize(), Color::green, hero.transform.getAngle());
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
            int indx = getAnimationIndex(item->animationLarge, current);
            drawTexture(item->animationLarge->frames.at(indx), item->dst);
        }

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
            drawText(Vec2(0, 0), "Paused", Color::white, pressStart, currentDisplayElement.fontSize);
            drawTexture(*currentDisplayElement.mainTexture, displayTexturePos, displayTexturesize);

            renderButton(displayElementExit);
        }
    }

    void close() {

    }

    void openItemImages() {
        //inventory textures
        smallWeapons[0] = loadTexture("./assets/images/items/weapon_small_01.png");
        smallWeapons[1] = loadTexture("./assets/images/items/weapon_small_01.png");
        smallWeapons[2] = loadTexture("./assets/images/items/weapon_small_01.png");

        smallArmour[0] = loadTexture("./assets/images/items/armour_small_01.png");
        smallArmour[1] = loadTexture("./assets/images/items/armour_small_01.png");
        smallArmour[2] = loadTexture("./assets/images/items/armour_small_01.png");

        //on ground animations
        armourAnimation = loadAnimation("./assets/images/items/armour_large_animation_grid.png", (Rect){0, 0, 20, 20}, 6, 2.0f, true);
        weaponAnimation = loadAnimation("./assets/images/items/weapon_large_animation_grid.png", (Rect){0, 0, 20, 20}, 6, 2.0f, true);
    }

    void displayDialogue(const char *msg) {
        Vec2 heroPos = hero.transform.getPosition(LOCAL);
        Vec2 msgSize = measureText(msg, pressStart, 14);
        Vec2 pos = Vec2(heroPos.x - (msgSize.x/2), heroPos.y - 30);
        drawText(pos, msg, Color::white, pressStart, 14, 0.0f);
    }

    Laser::Laser() {
        transform = Transform(hero.transform.getPosition(LOCAL), hero.transform.getAngle(), Vec2(13, 3));
        speed = 300.0f;
    };
}