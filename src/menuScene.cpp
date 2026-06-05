#include <menuScene.h>

namespace MenuScene {
    Font pressStart;
    std::vector<Button> buttons;
    Texture background;

    void init() {
        setWindowTitle("Pest Control - Menu");
        pressStart = loadFont("assets/fonts/PressStart2P-Regular.ttf");
        Texture buttonBackground = loadTexture("./assets/images/button_pink.png");
        background = loadTexture("./assets/images/backgrounM.png");

        buttons.push_back(createButton(
            Vec2((WINDOW_WIDTH/2), 500), Vec2(10, 10), "PLay Game", pressStart, 25.0f, Color::black, Color::white, START, buttonBackground
        ));
        
        buttons.push_back(createButton(
            Vec2((WINDOW_WIDTH/2), 600), Vec2(10, 10), "Quit", pressStart, 25.0f, Color::black, Color::white, QUIT, buttonBackground
        ));
    }

    void update(float dt) {
        for (Button &B : buttons) {
            // Cursor is over button
            if(collision(mousePosition(), B.position, B.size)) {
                B.hovered = true;

                // Mouse Click
                if(mouseButtonPressedThisFrame(MOUSE_BUTTON_LEFT)) {
                    switch (B.tag) {
                        case START:
                            // Load the Game
                            loadScene(GAME);
                            break;
                        case QUIT:
                            exit(0);
                            break;
                    }
                }
            } else {
                B.hovered = false;
            }
        }
    }

    void render(float lag) {
        clear((Color){50, 50, 50, 255});
        drawTexture(background, Vec2(0, 0), Vec2(WINDOW_WIDTH, WINDOW_HEIGHT));
        Vec2 titleSize = measureText("PEST CONTROL", pressStart, 40);

        drawText(Vec2((WINDOW_WIDTH/2)-(titleSize.x/2), 100), "PEST CONTROL", (Color){219, 0, 172, 255}, pressStart, 40);

        for (Button &B : buttons) {
            renderButton(B);
        }
    }

    void close() {

    }
}