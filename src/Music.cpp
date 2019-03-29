#include "../include/Music.h"

Music::Music(){
    music = nullptr;
}

Music::Music(std::string file){
    music = nullptr;
    Open(file);
}

Music::~Music(){
}

void Music::Open(std::string file){
    music = Resources::GetMusic(file);
    if(music == nullptr){
        std::cout << "Failed loading Music - " << SDL_GetError() << "\n" << std::endl;
    }
}

void Music::Play(int times){
    if(music){
        Mix_PlayMusic(music,times);
    }else{
        std::cout << "No Music Loaded to Play\n" << std::endl;
    }
}

void Music::Stop(int msToStop){
    Mix_FadeOutMusic(msToStop);

}

bool Music::IsOpen(){
    if(music){
        return true;
    }else{
        return false;
    }
}
