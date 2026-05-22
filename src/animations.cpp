#include <animations.h>

Animation loadAnimation(const char *fileName, Rect src, int numberFrames, float duration, bool loop) {
    Animation result;
    Texture spriteSheet = loadTexture(fileName);

    //load frames
    for (int i = 0; i < numberFrames; i++) {
        result.frames.push_back(subTexture(spriteSheet, (Rect){i*src.width, src.y, src.width, src.height}));
    }

    //set values
    result.numFrames = result.frames.size();
    result.duration = duration;
    result.loop = loop;

    return result;
}

int getAnimationIndex(Animation *animation, float time) {
    //calculate index
    int index = (time - animation->start) / animation->duration * animation->numFrames;

    if (animation->loop) {
        //mod to get looped index
        index %= animation->numFrames;
    } else {
        //clamp index to range
        index = SDL_clamp(index, 0, animation->numFrames-1);
    }

    //return the absolute value to prevent bugs
    return SDL_abs(index);
}

bool updateTimer(Timer* timer, float dt) {
    if (!timer->active) return false;

    if (timer->elasped >= timer->interval) {
        if (!timer->isCooldown) timer->elasped = 0.0f;
        return true;
    } else {
        timer->elasped += dt;
        return false;
    }
}

void activateTimer(Timer* timer, float interval, bool isCooldown, bool startActive) {
    timer->active = startActive;
    timer->interval = interval;
    timer->isCooldown = isCooldown;
}

void resetTimer(Timer* timer) {
    timer->elasped = 0.0f;
}