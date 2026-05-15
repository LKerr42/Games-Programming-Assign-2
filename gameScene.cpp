#include <gameScene.h>

namespace GameScene {
    Font pressStart;
    Texture temp;
    Hero hero;

    std::vector<Laser> lasers;
    std::vector<Wall> walls;

    Level currentLevel = LEVEL_ONE;

    void init() {
        setWindowTitle("Pest Control - Playing");
        pressStart = loadFont("assets/fonts/PressStart2P-Regular.ttf");

        hero.tex = loadTexture("./assets/images/hero.png");

        walls.push_back((Wall){
            300, 0, 200, 200,
            true, 
            NULL, NULL
        });
    }

    void update(float dt) {
        //Check for collisions, and then try to move player
        hero.transform.savePrevPos();

        if (keyIsPressed(KEY_W)) hero.transform.translate(Vec2(0, -3));
        if (keyIsPressed(KEY_A)) hero.transform.translate(Vec2(-3, 0));
        if (keyIsPressed(KEY_S)) hero.transform.translate(Vec2(0, 3));
        if (keyIsPressed(KEY_D)) hero.transform.translate(Vec2(3, 0));

        //check for wall collisions
        for (Wall W : walls) {
            if (collision(hero.transform.getBoundingBox(), 0.0f, 
                Vec2(W.x, W.y), Vec2(W.w, W.h), 0.0f)) {
                hero.transform.resetPos();
            }
        }

        hero.transform.rotateTo(mousePosition());
        hero.transform.updateBoundingBox();

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
            drawRect(W.x, W.y, W.w, W.h, Color::yellow);
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