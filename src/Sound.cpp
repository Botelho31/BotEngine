#include "../include/Sound.h"

Sound::Sound(GameObject& associated) : Component(associated){
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

void Sound::Open(std::string file){
    chunk = Mix_LoadWAV(file.c_str());
}

void Sound::Play(int times){
    channel = Mix_PlayChannel(-1,chunk,times);
}

void Sound::Stop(){
    if(chunk != nullptr){
        Mix_HaltChannel(channel);
    }
}

void Sound::Update(float dt){

}

void Sound::Render(){

}

bool Sound::Is(std::string file){
    if(file == "Sound"){
        return true;
    }else{
        return false;
    }
}
