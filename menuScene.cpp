#include <menuScene.h>

namespace MenuScene {
    Font pressStart;

    void init() {
        pressStart = loadFont("assets/fonts/PressStart2P-Regular.ttf");
    }

    void update(float dt) {

    }

    void render(float lag) {
        clear((Color){50, 50, 50, 255});

        drawText(Vec2(100, 100), "PEST CONTROL", (Color){219, 0, 172, 255}, pressStart, 24);
    }

    void close() {

    }
}