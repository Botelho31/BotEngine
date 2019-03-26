#include "../include/Sound.h"

Sound::Sound(GameObject& associated,Resources* resources) : Component(associated){
    chunk = nullptr;
    this->resources = resources;
    channel = -2;
}

Sound::Sound(GameObject& associated,Resources* resources, std::string file) : Sound(associated,resources){
    Open(file);
}

Sound::~Sound(){
    if(chunk != nullptr){
        if(channel != -2){
            Mix_HaltChannel(channel);
        }
        Mix_FreeChunk(chunk);
    }
}

void Sound::Open(std::string file){
    chunk = Mix_LoadWAV(file.c_str());
}

void Sound::Play(int times){
    channel = Mix_PlayChannel(-1,chunk,times);
}

bool Sound::IsPlaying(){
    if(Mix_Playing(channel)){
        return true;
    }else{
        return false;
    }
}

bool Sound::IsOpen(){
    if(chunk){
        return true;
    }else{
        return false;
    }
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
