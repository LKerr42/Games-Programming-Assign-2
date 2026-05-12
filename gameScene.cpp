#include <gameScene.h>

namespace GameScene {
    Font pressStart;
    Texture temp;
    Hero hero;

    std::vector<Laser> lasers;

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

        //calculate bounding box
        float minX = FLT_MAX, maxX = -FLT_MAX, minY = FLT_MAX, maxY = -FLT_MAX;
        float angleRads = hero.angle * M_PI / 180.0f;

        Vec2 cornerPos[4] = {
            Vec2(-hero.size.x/2, hero.size.y/2),
            Vec2(hero.size.x/2, hero.size.y/2),
            Vec2(hero.size.x/2, -hero.size.y/2),
            Vec2(-hero.size.x/2, -hero.size.y/2)
        };

        for (int i = 0; i < 4; i++) {
            Vec2 currentPos = cornerPos[i];
            cornerPos[i].x = (currentPos.x * cos(angleRads) - currentPos.y * sin(angleRads)) + hero.position.x;
            cornerPos[i].y = (currentPos.x * sin(angleRads) + currentPos.y * cos(angleRads)) + hero.position.y;

            minX = min(minX, cornerPos[i].x);
            minY = min(minY, cornerPos[i].y);
            maxX = max(maxX, cornerPos[i].x);
            maxY = max(maxY, cornerPos[i].y);
        }

        hero.boundingBox = (Rect){
            minX + (hero.size.x/2), minY + (hero.size.y/2),
            maxX - minX,
            maxY - minY
        };

        //update lasers
        if (keyPressedThisFrame(KEY_SPACE)) lasers.emplace_back();

        for (int i = 0; i < lasers.size();) {
            Laser &L = lasers.at(i);
            float angleRad = L.angle * M_PI / 180.0f;

            L.position.x += cos(angleRad) * L.speed * dt;
            L.position.y += sin(angleRad) * L.speed * dt;

            if (L.position.x > WINDOW_WIDTH || L.position.x < 0 || L.position.y > WINDOW_HEIGHT || L.position.y < 0) {
                lasers.erase(lasers.begin() + i);
            } else {
                i++;
            }
        }
    }

    void render(float lag) {
        clear((Color){50, 50, 50, 255});

        drawText(Vec2(10, 10), "Gaming time", (Color){219, 0, 172, 255}, pressStart, 24);

        drawRect(hero.position, hero.size, Color::green, hero.angle);
        drawRect(hero.getBBpos(), hero.getBBsize(), Color::red);
        drawTexture(hero.tex, hero.position, hero.size, hero.angle);

        for (Laser &L : lasers) {
            fillRect(L.position, L.size, Color::green, L.angle);
        }
    }

    void close() {

    }

    Hero::Hero() {
        position = Vec2(100, WINDOW_HEIGHT/2);
        size = Vec2(40, 25);
        boundingBox = (Rect){position.x, position.y, size.x, size.y};
        health = 100;
        angle = 0.0f;
    };

    Laser::Laser() {
        position = hero.position;
        size = Vec2(10, 3);
        angle = hero.angle;
        speed = 300.0f;
    };
}