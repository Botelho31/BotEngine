#include "../include/Sprite.h"
#include "../include/State.h"
#include "../include/Camera.h"

Sprite::Sprite(GameObject& associated,int frameTime,float frameCount) : 
    Component(associated),frameTime(frameTime),frameCount(frameCount){
    scale.x = 1;
    scale.y = 1;
    texture = nullptr;
    associated.angleDeg = 0;
}

Sprite::Sprite(GameObject& associated,std::string file,int frameTime,float frameCount) :
    Sprite(associated,frameTime,frameCount){

    Open(file);
}

Sprite::~Sprite(){
}

void Sprite::Open(std::string file){
    texture = Resources::GetImage(file);
    if(texture == nullptr){
        std::cout << "Error Loading Image: " << SDL_GetError() << std::endl;
        return;
    }else{
        SDL_QueryTexture(texture,nullptr,nullptr, &width, &height);
        this->associated.box.w = width/frameCount;
        this->associated.box.h = height/frameCount;
        SetClip(0,0,width,height);
        
    }
}

void Sprite::SetClip(int x,int y,int w,int h){
    clip_rect.h = h;
    clip_rect.w = w;
    clip_rect.x = x;
    clip_rect.y = y;
}

void Sprite::SetScaleX(float scaleX,float scaleY){
    if(scaleX != 0){
        scale.x = scaleX;
        associated.box.x = associated.box.x + (associated.box.w - (associated.box.w*scaleX));
        associated.box.w *= scaleX;
    }
    if(scaleY != 0){
        scale.y = scaleY;
        associated.box.y = associated.box.y + (associated.box.h - (associated.box.h*scaleY));
        associated.box.h *= scaleY;
    }
}

void Sprite::SetFrame(int frame){
    currentFrame = frame;
    SetClip(width*currentFrame,0,width,height);
}

void Sprite::SetFrameCount(int frameCount){
    currentFrame = 0;
    this->frameCount = frameCount;
}

void Sprite::SetFrameTime(float frameTime){
    this->frameTime = frameTime;
}

void Sprite::Update(float dt){
    timeElapsed += dt;
    if(timeElapsed >= frameTime ){
        currentFrame += 1;
        if(currentFrame >= frameCount){
            currentFrame = 0;
        }
        SetClip((width/frameCount)*currentFrame,0,width/frameCount,height);
    }

}

void Sprite::Render(){
    Render(this->associated.box.x - Camera::pos.x,this->associated.box.y - Camera::pos.y);
}

void Sprite::Render(int x,int y){
    SDL_Rect dst_rect;
    dst_rect.x = x;
    dst_rect.y = y;
    dst_rect.w = clip_rect.w * scale.x;
    dst_rect.h = clip_rect.h * scale.y;
    SDL_RenderCopyEx(Game::GetInstance().GetRenderer(),texture,&clip_rect,&dst_rect,associated.angleDeg,nullptr,SDL_FLIP_NONE);
}

int Sprite::GetWidth(){
    return (width * scale.x) / frameCount;
}

int Sprite::GetHeight(){
    return height * scale.y;
}

bool Sprite::IsOpen(){
    if(texture){
        return true;
    }else{
        return false;
    }
}

bool Sprite::Is(std::string type){
    if(type == "Sprite"){
        return true;
    }else{
        return false;
    }
}