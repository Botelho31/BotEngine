#include "../include/Sound.h"

Sound::Sound(GameObject& associated) : Component(associated){
    this->associated = associated;
    chunk = nullptr;
}

Sound::Sound(GameObject& associated, std::string file) : Sound(associated){
    Open(file);
}

Sound::~Sound(){
    if(chunk != nullptr){
        Mix_HaltChannel(channel);
        Mix_FreeChunk(chunk);
    }
}

void Sound::Play(int times = 1){
    channel = Mix_PlayChannel(-1,chunk,times);
}

void Sound::Stop(){
    if(chunk != nullptr){
        Mix_HaltChannel(channel);
    }
}
