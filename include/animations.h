#ifndef ANIMATIONS_H_
#define ANIMATIONS_H_

#include <engine.h>

#include <vector>

//struct of the representation of a timer, stores when started, length, and if it is active
typedef struct Timer {
    float elasped, interval;
    int presses;
    bool active, isCooldown;
} Timer;

//struct of the representation of an animation, with a vector array of frames and neccesary values
typedef struct Animation {
    std::vector<Texture> frames;
    int numFrames;
    float duration, start;
    bool loop;
    Vec2 dst;
} Animation;

Animation loadAnimation(const char *fileName, Rect src, int numberFrames, float duration, bool loop);
int getAnimationIndex(Animation *animation, float time, bool* over);
bool updateTimer(Timer* timer, float dt);
void activateTimer(Timer* timer, float interval, bool isCooldown, bool startActive);
void resetTimer(Timer* timer);

#endif