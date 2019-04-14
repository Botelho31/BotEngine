#include "../include/WindowEffects.h"
#include "../include/Camera.h"

WindowEffects::WindowEffects(){
    currentEffect = NOTHING;
    currenteffectseconds = 0;
}

WindowEffects::~WindowEffects(){

}

void WindowEffects::Update(float dt){
    switch(currentEffect){
        case FADETOBLACK:
            float alphapersecond = 255/currenteffectseconds;
            if((a + (alphapersecond *dt)) >= 255){
                a = 255;
                currentEffect = NOTHING;
            }else{
                a += alphapersecond * dt;
            }
            DrawToScreen(r,g,b,a);
            break;
        case FADEFROMBLACK:
            float alphapersecond = -(255/currenteffectseconds);
            if((a + (alphapersecond *dt)) <= 0){
                a = 0;
                currentEffect = NOTHING;
            }else{
                a += alphapersecond * dt;
            }
            DrawToScreen(r,g,b,a);
            break;
        default:
            break;
    }
}

void WindowEffects::FadeToBlack(int seconds){
    r = 0;
    g = 0;
    b = 0;
    a = 0;
    this->currenteffectseconds = seconds;
    this->currentEffect = FADETOBLACK;
}

void WindowEffects::FadeFromBlack(int seconds){
    r = 0;
    g = 0;
    b = 0;
    a = 255;
    this->currenteffectseconds = seconds;
    this->currentEffect = FADEFROMBLACK;
}

void WindowEffects::DrawToScreen(int r,int g,int b,int a){
    SDL_SetRenderDrawBlendMode(Game::GetInstance().GetRenderer(),SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(Game::GetInstance().GetRenderer(),r,g,b,a);
    SDL_Rect rect;
    rect.x = 0;
    rect.y = 0;
    rect.w = Camera::window.x;
    rect.h = Camera::window.y;
    SDL_RenderFillRect(Game::GetInstance().GetRenderer(),&rect);
}

bool WindowEffects::Drawing(){
    if(currentEffect == NOTHING){
        return false;
    }else{
        return true;
    }
}