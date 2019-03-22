#include "../include/State.h"
#include "../include/Sprite.h"
#include "../include/Music.h"

State::State(){
    quitRequested = false;
    music = new Music("assets/audio/stageState.ogg");
    music->Play();
    bg = new Sprite("assets/img/ocean.jpg");
    bg->SetClip(0,0,bg->GetWidth(),bg->GetHeight());
    bg->Render(0,0);
}

void State::LoadAssets(){

}

void State::Update(float dt){
    dt = 0;
    dt ++;
    if(SDL_QuitRequested()){
        quitRequested = true;
    }
}

void State::Render(){
}

bool State::QuitRequested(){
    return quitRequested;
}