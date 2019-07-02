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

void Music::Play(int times,int ms){
    if(music){
        Mix_FadeInMusic(music.get(),times,ms);
    }else{
        std::cout << "No Music Loaded to Play\n" << std::endl;
    }
}

void Music::Stop(int msToStop){
    Mix_FadeOutMusic(msToStop);

}

void Music::Pause(){
    if(Mix_PlayingMusic()){
        Mix_PauseMusic();
    }
}

void Music::Resume(){
    if(Mix_PausedMusic()){
        Mix_ResumeMusic();
    }
}

bool Music::IsOpen(){
    if(music){
        return true;
    }else{
        return false;
    }
}
