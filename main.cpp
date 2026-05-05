#include <engine.h>
#include <iostream>
#include <vector>

// Initialise (called once at start)
void init() {
    // Example
    setWindowTitle("[game name] - Menu");

}

// Update Game
void update(float dt) {

}

// Render Game
void render(float lag) {
    drawRect(Vec2(0, 0), Vec2(100, 100), Color::blue);
}

// Close the Game
void close() {

}