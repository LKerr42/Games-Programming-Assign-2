#include <gameScene.h>

namespace GameScene {
    Font pressStart;
    Hero hero;

    std::vector<Laser> lasers;
    std::vector<Wall> walls;

    Level currentLevel = LEVEL_ONE;

    void init() {
        setWindowTitle("Pest Control - Playing");
        pressStart = loadFont("assets/fonts/PressStart2P-Regular.ttf");

        hero.tex = loadTexture("./assets/images/hero.png");

        walls.push_back((Wall){
            (Rect){300, 0, 200, 200},
            true, 
            NULL, NULL
        });
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
        if (mouseButtonPressedThisFrame(MOUSE_BUTTON_LEFT)) lasers.emplace_back();

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
        clear((Color){50, 50, 50, 255});

        drawText(Vec2(10, 10), "Gaming time", (Color){219, 0, 172, 255}, pressStart, 24);

        drawRect(hero.transform.getBoundingBox(), Color::red, 0.0f);
        drawRect(hero.transform.getPosition(), hero.transform.getSize(), Color::green, hero.transform.getAngle());
        drawTexture(hero.tex, hero.transform.getPosition(), hero.transform.getSize(), hero.transform.getAngle());

        for (Laser &L : lasers) {
            fillRect(L.transform.getPosition(), L.transform.getSize(), Color::green, L.transform.getAngle());
        }

        for (Wall &W : walls) {
            drawRect(W.rect, Color::yellow);
        }
    }

    void close() {

    }

    Hero::Hero() {
        transform = Transform(Vec2(100, WINDOW_HEIGHT/2), 0.0f, Vec2(40, 25));
        health = 100;
    };

    Laser::Laser() {
        transform = Transform(hero.transform.getPosition(LOCAL), hero.transform.getAngle(), Vec2(13, 3));
        speed = 300.0f;
    };
}