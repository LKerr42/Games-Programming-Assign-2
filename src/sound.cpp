#include <sound.h>
#include <engine.h>
#include <iostream>



void loadLaserShoot(audioClips audio, char *shoot) {
    audio.laserShoot = loadAudioClip(shoot);
}

void loadLaserHit(audioClips audio, char *hit) {
    audio.laserHit = loadAudioClip(hit); 
}

void loadAlienSounds(audioClips audio, char *noises) {
    audio.AlienSounds = loadAudioClip(noises);
}

void playLaserShoot(audioClips audio) {
    playOnce(audio.laserShoot);
}

void playLaserHit(audioClips audio) {
    playOnce(audio.laserHit);
}

void playAlienSounds(audioClips audio) {
    playAudio(audio.AlienSounds);
}

void stopAlienSounds(audioClips audio) {
    stopAudio(audio.AlienSounds);
}