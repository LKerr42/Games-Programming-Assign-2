#include <button.h>

//Create a new button struct instance with texture background
Button createButton(Vec2 pos, Vec2 padding, const char *str, Font F, float pSize, Color C, Color sC, Tag T, Texture backTex) {
    Button newButton;

    newButton.size = measureText(str, F, pSize) + (padding * 2);
    newButton.position = Vec2(pos.x - (newButton.size.x/2), pos.y - (newButton.size.y/2));
    
    newButton.text = str;
    newButton.font = F;
    newButton.ptSize = pSize;
    newButton.colour = C;
    newButton.selectedColour = sC;
    newButton.tag = T;

    newButton.hovered = false;
    newButton.selected = false;
    newButton.hasTexture = true;

    newButton.background = backTex;

    return newButton;
}

Button createButton(Vec2 pos, Vec2 padding, const char *str, Font F, float pSize, Color C, Color sC, Tag T, Color backCol) {
    Button newButton;

    newButton.size = measureText(str, F, pSize) + (padding * 2);
    newButton.position = Vec2(pos.x - (newButton.size.x/2), pos.y - (newButton.size.y/2));

    newButton.text = str;
    newButton.font = F;
    newButton.ptSize = pSize;
    newButton.colour = C;
    newButton.selectedColour = sC;
    newButton.tag = T;

    newButton.hovered = false;
    newButton.selected = false;
    newButton.hasTexture = false;

    newButton.backgroundColour = backCol;

    return newButton;
}


//Render a button instance
void renderButton(Button &B) {
    Vec2 messageSize = measureText(B.text, B.font, B.ptSize);

    Color colour = (B.hovered) ? B.selectedColour : B.colour;

    if(B.hasTexture) {
        drawTexture(B.background, B.position, B.size);
    } else {
        fillRect(B.position, B.size, B.backgroundColour);
    }
    drawText(B.position + B.size/2 - messageSize/2, B.text, colour, B.font, B.ptSize);
}