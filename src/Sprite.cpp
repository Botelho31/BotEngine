#include "../include/Sprite.h"
#include "../include/State.h"
#include "../include/Game.h"

Sprite::Sprite(){
    texture = nullptr;
}

Sprite::Sprite(const char* file){
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
    }
}

void Sprite::SetClip(int x,int y,int w,int h){
    clip_rect.h = h;
    clip_rect.w = w;
    clip_rect.x = x;
    clip_rect.y = y;
}

void Sprite::Render(int x,int y){
    SDL_Rect dst_rect;
    dst_rect.x = x;
    dst_rect.y = y;
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