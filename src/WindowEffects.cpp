#include "../include/WindowEffects.h"
#include "../include/Camera.h"

WindowEffects::WindowEffects(){
    currentEffect = NOTHING;
    currenteffectseconds = 0;
}

WindowEffects::~WindowEffects(){

}

void WindowEffects::Update(float dt){
    float alphapersecond = 0;
    switch(currentEffect){
        case FADETOBLACK:
            alphapersecond = 255/currenteffectseconds;
            if((a + (alphapersecond *dt)) >= 255){
                a = 255;
                currentEffect = NOTHING;
                currenteffectseconds = 0;
            }else{
                a += alphapersecond * dt;
            }
            DrawToScreen(r,g,b,a);
            break;
        case FADEFROMBLACK:
            alphapersecond = -(255/currenteffectseconds);
            if((a + (alphapersecond *dt)) <= 0){
                a = 0;
                currentEffect = NOTHING;
                currenteffectseconds = 0;
            }else{
                a += alphapersecond * dt;
            }
            DrawToScreen(r,g,b,a);
            break;
        case NOTHING:
            break;
        default:
            break;
    }
}

void WindowEffects::Render(){
    if(Drawing()){
        DrawToScreen(r,g,b,a);
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

bool WindowEffects::IsBlack(){
    if((a == 255) && (r == 0) && (g == 0) && (b == 0)){
        return true;
    } else{
        return false;
    }
}