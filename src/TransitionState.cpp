#include "../include/TransitionState.h"
#include "../include/Camera.h"

TransitionState::TransitionState(float secondsToPop){
    this->secondsToPop = secondsToPop;
    this->currentalpha = 0;
    this->alphapersecond = 0;
}
TransitionState::~TransitionState(){

}
void TransitionState::LoadAssets(){

}
void TransitionState::Update(float dt){
    poptimer->Update(dt);
    if((currentalpha + (alphapersecond *dt)) >= 255){
        currentalpha = 255;
        alphapersecond = -alphapersecond;
    }
    else if((currentalpha + (alphapersecond *dt)) <= 0){
        currentalpha = 0;
        alphapersecond = 0;
    }else{
        currentalpha += alphapersecond * dt;
    }
    FadeToBlack(currentalpha);
    if(poptimer->Get() >= secondsToPop){
        popRequested = true;
    }
}
void TransitionState::Render(){

}

void TransitionState::Start(){
    alphapersecond = (255*2)/secondsToPop;
}
void TransitionState::Resume(){

}
void TransitionState::Pause(){

}
void TransitionState::FadeToBlack(int alpha){
    SDL_SetRenderDrawBlendMode(Game::GetInstance().GetRenderer(),SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(Game::GetInstance().GetRenderer(),0,0,0,alpha);
    SDL_Rect rect;
    rect.x = 0;
    rect.y = 0;
    rect.w = Camera::window.x;
    rect.h = Camera::window.y;
    SDL_RenderFillRect(Game::GetInstance().GetRenderer(),&rect);
}