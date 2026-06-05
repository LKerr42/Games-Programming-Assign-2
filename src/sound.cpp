#include <sound.h>
#include <engine.h>
#include <iostream>



void loadLaserShoot(audioClips &audio, char *shoot) {
    audio.laserShoot = loadAudioClip(shoot);
}

void loadLaserHit(audioClips &audio, char *hit) {
    audio.laserHit = loadAudioClip(hit); 
}

void loadAlienSounds(audioClips &audio, char *noises) {
    audio.AlienSounds = loadAudioClip(noises);
}

void playLaserShoot(audioClips &audio) {
    playOnce(audio.laserShoot, 0.4f);
}

void playLaserHit(audioClips &audio) {
    playOnce(audio.laserHit, 0.5f);
}

void playAlienSounds(audioClips &audio) {
    playAudio(audio.AlienSounds, 0.5f, true);
}

void stopAlienSounds(audioClips &audio) {
    stopAudio(audio.AlienSounds);
}

void loadHurtSound(audioClips &audio, char *hurt) {
    audio.Hurt = loadAudioClip(hurt);
}

void playHurtSound(audioClips &audio) {
    playOnce(audio.Hurt, 0.3f);
}