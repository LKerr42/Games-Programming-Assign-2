#ifndef sound_h_
#define sound_h_

#include <engine.h>

struct audioClips {
    AudioClip laserShoot;
    AudioClip laserHit;
    AudioClip AlienSounds;
    AudioClip Hurt;
    AudioClip music;
};

void loadMusic(audioClips &audio, char *music);

void loadLaserShoot(audioClips &audio, char *shoot);

void loadLaserHit(audioClips &audio, char *hit);

void loadAlienSounds(audioClips &audio, char *noises);

void loadHurtSound(audioClips &audio, char *hurt);

void playHurtSound(audioClips &audio);


void playLaserShoot(audioClips &audio);

void playLaserHit(audioClips &audio);

void playAlienSounds(audioClips &audio);

void stopAlienSounds(audioClips &audio);


#endif