#include <menuScene.h>

namespace MenuScene {
    Font pressStart;
    std::vector<Button> buttons;

    void init() {
        pressStart = loadFont("assets/fonts/PressStart2P-Regular.ttf");
        Texture buttonBackground = loadTexture("./assets/images/button_rectangle_depth_gloss.png");


        buttons.push_back(createButton(
            Vec2(50, 50), Vec2(250, 50), "PLay Game", pressStart, 25.0f, Color::black, Color::white, START, buttonBackground
        ));
        
        buttons.push_back(createButton(
            Vec2(50, 200), Vec2(10, 10), "Quit", pressStart, 25.0f, Color::black, Color::white, QUIT, Color::red
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

        //drawText(Vec2(100, 100), "PEST CONTROL", (Color){219, 0, 172, 255}, pressStart, 24);

        for (Button &B : buttons) {
            renderButton(B);
        }
    }

    void close() {

    }
}