#include <gameScene.h>

#define HUD_PIXEL_SIZE 8

namespace GameScene {
    Font pressStart;
    Texture hudBase;
    Rect hudDest;
    Hero hero;

    std::vector<Laser> lasers;
    std::vector<Wall> walls;
    std::vector<Item*> levelItems;

    Level currentLevel = LEVEL_ONE;
    DisplayState currentDisplay = HUD;
    Texture weapons[6], armours[6], upgrades[6], background;


    void init() {
        setWindowTitle("Pest Control - Playing");
        pressStart = loadFont("assets/fonts/PressStart2P-Regular.ttf");
        background = loadTexture("assets/images/background.png");

        //init hero
        hero.tex = loadTexture("./assets/images/hero.png");
        SDL_SetTextureScaleMode(hero.tex.texture, SDL_SCALEMODE_NEAREST);

        //init items
        //large
        weapons[0] = loadTexture("./assets/images/weapon_large_01.png");
        weapons[1] = loadTexture("./assets/images/weapon_large_01.png");
        weapons[2] = loadTexture("./assets/images/weapon_large_01.png");

        armours[0] = loadTexture("./assets/images/armour_large_01.png");
        armours[1] = loadTexture("./assets/images/armour_large_01.png");
        armours[2] = loadTexture("./assets/images/armour_large_01.png");

        //small
        weapons[3] = loadTexture("./assets/images/weapon_large_01.png");
        weapons[4] = loadTexture("./assets/images/weapon_large_01.png");
        weapons[5] = loadTexture("./assets/images/weapon_large_01.png");

        armours[3] = loadTexture("./assets/images/armour_large_01.png");
        armours[4] = loadTexture("./assets/images/armour_large_01.png");
        armours[5] = loadTexture("./assets/images/armour_large_01.png");
        for (int i = 0; i < 6; i++) {
            SDL_SetTextureScaleMode(weapons[i].texture, SDL_SCALEMODE_NEAREST);
            SDL_SetTextureScaleMode(armours[i].texture, SDL_SCALEMODE_NEAREST);
        }
        
        hero.currWeapon = new Weapon(&weapons[0], &weapons[3], (Rect){0, 0, 0, 0}, 100, 100, 100);

        levelItems.push_back(new Armour(&armours[0], &armours[3], (Rect){640, WINDOW_HEIGHT/2, 0, 0}, 100, 100));
        levelItems.push_back(new Weapon(&weapons[0], &weapons[3], (Rect){700, WINDOW_HEIGHT/2, 0, 0}, 100, 100, 100));

        //init Hud
        hudBase = loadTexture("./assets/images/hud_base.png");
        float hudWidth = 128 * 8, hudHeight = 72 * 8;
        hudDest = (Rect){0, 0, hudWidth, hudHeight};
        SDL_SetTextureScaleMode(hudBase.texture, SDL_SCALEMODE_NEAREST);

        //init level
        walls.push_back((Wall){(Rect){WINDOW_WIDTH-900, 0, 300, 200}, true, NULL, NULL});
        walls.push_back((Wall){(Rect){WINDOW_WIDTH-600, 0, 300, 200}, true, NULL, NULL});
        walls.push_back((Wall){(Rect){WINDOW_WIDTH-300, 0, 300, 200}, true, NULL, NULL});

        walls.push_back((Wall){(Rect){WINDOW_WIDTH-900, WINDOW_HEIGHT-200, 300, 200}, true, NULL, NULL});
        walls.push_back((Wall){(Rect){WINDOW_WIDTH-600, WINDOW_HEIGHT-200, 300, 200}, true, NULL, NULL});
        walls.push_back((Wall){(Rect){WINDOW_WIDTH-300, WINDOW_HEIGHT-200, 300, 200}, true, NULL, NULL});

        std::cout << "width: " << WINDOW_WIDTH << " height: " << WINDOW_HEIGHT << "\n";
    }

    void update(float dt) {
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
        if (keyIsPressed(KEY_R)) hero.currWeapon->currEnergy = hero.currWeapon->fullEnergy;

        if (mouseButtonPressedThisFrame(MOUSE_BUTTON_LEFT) && hero.currWeapon->currEnergy > 0) {
            lasers.emplace_back();
            hero.currWeapon->currEnergy -= 10;
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
    }

    void render(float lag) {
        //clear((Color){50, 50, 50, 255});

        //drawText(Vec2(10, 10), "Gaming time", (Color){219, 0, 172, 255}, pressStart, 24);

        drawTexture(background, Vec2(0, 0), Vec2(WINDOW_WIDTH, WINDOW_HEIGHT));

        //hero
        drawRect(hero.transform.getBoundingBox(), Color::red, 0.0f);
        drawRect(hero.transform.getPosition(), hero.transform.getSize(), Color::green, hero.transform.getAngle());
        drawTexture(hero.tex, hero.transform.getPosition(), hero.transform.getSize(), hero.transform.getAngle());

        //lasers
        for (Laser &L : lasers) {
            fillRect(L.transform.getPosition(), L.transform.getSize(), Color::green, L.transform.getAngle());
        }

        //walls
        for (Wall &W : walls) {
            drawRect(W.rect, Color::yellow);
        }

        //display items
        for (Item* item : levelItems) {
            drawTexture(*item->largeTexture, item->dst);
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
            //if (hero.currWeapon != nullptr) drawTexture(*hero.currWeapon->tex, (Rect){22+4, 180+4, 30, 30});
        } else {
            fillRect(Vec2(0, 0), Vec2(WINDOW_WIDTH, WINDOW_HEIGHT), (Color){0, 0, 0, 128});
        }
    }

    void close() {

    }

    Laser::Laser() {
        transform = Transform(hero.transform.getPosition(LOCAL), hero.transform.getAngle(), Vec2(13, 3));
        speed = 300.0f;
    };
}