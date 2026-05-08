#include <gameScene.h>

namespace GameScene {
    Font pressStart;
    Texture temp;
    Hero hero;

    void init() {
        setWindowTitle("Pest Control - Playing");
        pressStart = loadFont("assets/fonts/PressStart2P-Regular.ttf");

        hero.tex = loadTexture("./assets/images/hero.png");
    }

    void update(float dt) {
        //Move hero
        if (keyIsPressed(KEY_W)) hero.position.y -= 3;
        if (keyIsPressed(KEY_A)) hero.position.x -= 3;
        if (keyIsPressed(KEY_S)) hero.position.y += 3;
        if (keyIsPressed(KEY_D)) hero.position.x += 3;

        // Update hero angle
        Vec2 mousePos = mousePosition();

        Vec2 forward = mousePos - hero.position;

        hero.angle = atan2(forward.y, forward.x) / M_PI * 180.0f;
    }

    void render(float lag) {
        clear((Color){50, 50, 50, 255});

        drawText(Vec2(10, 10), "Gaming time", (Color){219, 0, 172, 255}, pressStart, 24);

        drawRect(hero.position, hero.size, Color::red, hero.angle);
        drawTexture(hero.tex, hero.position, hero.size, hero.angle);
    }

    void close() {

    }

    Hero::Hero() {
        position = Vec2(100, 100);
        size = Vec2(50, 33);
        health = 100;
        angle = 0.0f;
    };
}