#include "../include/Sprite.h"
#include "../include/State.h"

Sprite::Sprite(GameObject& associated) : Component(associated){
    texture = nullptr;
}

Sprite::Sprite(GameObject& associated,std::string file) : Sprite(associated){
    Open(file);
}

Sprite::~Sprite(){
    if(texture){
        SDL_DestroyTexture(texture);
    }
}

void Sprite::Open(std::string file){
    if(texture){
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
    texture = IMG_LoadTexture(Game::GetInstance().GetRenderer(), file.c_str());
    if(texture == nullptr){
        std::cout << "Error Loading Image: " << SDL_GetError() << std::endl;
        return;
    }else{
        SDL_QueryTexture(texture,nullptr,nullptr, &width, &height);
        this->associated.box.w = width;
        this->associated.box.h = height;
        SetClip(0,0,width,height);
        
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
    Render(this->associated.box.x,this->associated.box.y);
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

bool Sprite::Is(std::string type){
    if(type == "Sprite"){
        return true;
    }else{
        return false;
    }
}