#include "../include/Light.h"
#include "../include/Camera.h"

Light::Light(){
    this->windoweffects = new WindowEffects();
    lightsources.clear();
    lightsources.push_back(Circle(500 - Camera::pos.x,1000 - Camera::pos.y,300));
}
Light::~Light(){

}
void Light::Update(float dt){

}
void Light::Render(){
    DimScreen(200);
    for(int i = 0;i < lightsources.size();i++){
        SourceOfLight(lightsources[i].Added(-Camera::pos.x,-Camera::pos.y),0.1);
    }
}

void Light::DimScreen(int alpha){
    windoweffects->FillRect({0,0,Camera::limit.x,Camera::limit.y},0,0,0,alpha);
}
void Light::SourceOfLight(Circle circle,float intensity){
    // SDL_SetRenderDrawBlendMode(Game::GetInstance().GetRenderer(),SDL_BLENDMODE_ADD);
    int radius = circle.radius;
    float alphaest = 0;
    int alpha = 0;
    for(int i = 0;i < radius;i++){
        windoweffects->DrawCircle(circle,255,255,255,alpha);
        circle.radius --;
        alphaest += intensity;
        alpha = floor(alphaest);
        if(alpha > 255){
            alpha = 255;
        }
    }
    SDL_SetRenderDrawBlendMode(Game::GetInstance().GetRenderer(),SDL_BLENDMODE_BLEND);
}