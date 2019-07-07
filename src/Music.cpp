#include "../include/Music.h"

Music::Music(){
    music = nullptr;
    timer = new Timer();
    playing = false;
}

Music::Music(std::string file){
    music = nullptr;
    Open(file);
}

Music::~Music(){
    delete timer;
}

void Music::Update(float dt){
    if(playing){
        timer->Update(dt);
    }
}

void Music::Open(std::string file){
    music = Resources::GetMusic(file);
    if(music == nullptr){
        std::cout << "Failed loading Music - " << SDL_GetError() << "\n" << std::endl;
    }
}

void Music::Play(int times,int ms,bool maintainPos){
    if(music){
        if(maintainPos){
            Mix_FadeInMusic(music.get(),times,ms);
            Mix_FadeInMusicPos(music.get(),times,ms,timer->Get());
            playing = true;
            std::cout << "repeat" << std::endl;
            std::cout << timer->Get() << std::endl;
        }else{
            Mix_FadeInMusic(music.get(),times,ms);
            timer->Restart();
            playing = true;
        }
    }else{
        std::cout << "No Music Loaded to Play\n" << std::endl;
    }
}

void Music::Stop(int msToStop){
    Mix_FadeOutMusic(msToStop);
    playing = false;
}

void Music::Pause(){
    if(Mix_PlayingMusic()){
        Mix_PauseMusic();
        playing = false;
    }
}

void Music::Resume(){
    if(Mix_PausedMusic()){
        Mix_ResumeMusic();
        playing = true;
    }
}

float Music::GetElapsedTime(){
    return timer->Get();
}

bool Music::IsOpen(){
    if(music){
        return true;
    }else{
        return false;
    }
}
