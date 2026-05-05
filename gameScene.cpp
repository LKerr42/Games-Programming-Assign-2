#include <gameScene.h>

namespace GameScene {
    Font pressStart;

    void init() {
        pressStart = loadFont("assets/fonts/PressStart2P-Regular.ttf");
    }

    void update(float dt) {

    }

    void render(float lag) {
        clear(Color::white);

        drawText(Vec2(100, 100), "Gaming time", (Color){219, 0, 172, 255}, pressStart, 24);
    }

    void close() {

    }
}