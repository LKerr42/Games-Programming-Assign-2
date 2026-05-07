#ifndef BUTTON_H_
#define BUTTON_H_

#include <engine.h>

enum Tag {
    START,
    QUIT
};

typedef struct Button {
    Vec2 position;
    Vec2 size;
    const char *text;
    Font font;
    float ptSize;
    bool hovered;
    bool selected;
    Color colour;
    Color backgroundColour;
    Color selectedColour;
    Tag tag;
    Texture background;
    bool hasTexture;
} Button;

Button createButton(Vec2 pos, Vec2 S, const char *str, Font F, float pSize, Color C, Color sC, Tag T, Texture backTex);
Button createButton(Vec2 pos, Vec2 padding, const char *str, Font F, float pSize, Color C, Color sC, Tag T, Color backCol);
void renderButton(Button &B);

#endif