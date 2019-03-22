#include "../include/Sprite.h"
#include "../include/State.h"

Sprite::Sprite(GameObject& associated) : Component(associated){
    this->associated = associated;
    texture = nullptr;
}

Sprite::Sprite(GameObject& associated,const char* file): Component(associated){
    this->associated = associated;
    texture = nullptr;
    Open(file);
}

Sprite::~Sprite(){
    if(texture){
        SDL_DestroyTexture(texture);
    }
}

void Sprite::Open(const char* file){
    if(texture){
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
    texture = IMG_LoadTexture(Game::GetInstance().GetRenderer(), file);
    if(texture == nullptr){
        std::cout << "Error Loading Image: " << SDL_GetError() << std::endl;
        return;
    }else{
        SDL_QueryTexture(texture,nullptr,nullptr, &width, &height);
        this->associated.box.w = width;
        this->associated.box.h = height;
        
    }
}

void Sprite::SetClip(int x,int y,int w,int h){
    clip_rect.h = h;
    clip_rect.w = w;
    clip_rect.x = x;
    clip_rect.y = y;
}

void Sprite::Update(float dt){

}

void Sprite::Render(){
    SDL_Rect dst_rect;
    dst_rect.x = this->associated.box.x;
    dst_rect.y = this->associated.box.y;
    dst_rect.w = clip_rect.w;
    dst_rect.h = clip_rect.h;
    SDL_RenderCopy(Game::GetInstance().GetRenderer(),texture,&clip_rect,&dst_rect);
}

int Sprite::GetWidth(){
    return width;
}

int Sprite::GetHeight(){
    return height;
}

bool Sprite::IsOpen(){
    if(texture){
        return true;
    }else{
        return false;
    }
}

bool Sprite::Is(std::string){

}