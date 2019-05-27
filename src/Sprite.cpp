#include "../include/Sprite.h"
#include "../include/State.h"
#include "../include/Camera.h"

Sprite::Sprite(GameObject& associated,int frameCount,float frameTime,float secondsToSelfDestruct,bool repeat,bool singularTexture) : 
    Component(associated){
    this->frameCount = frameCount;
    this->frameTime = frameTime;
    this->secondsToSelfDestruct = secondsToSelfDestruct;
    this->selfDestructCount = new Timer();
    this->flip = false;
    this->repeat = repeat;
    this->singularTexture = singularTexture;
    selfDestructCount->Restart();
    scale.x = 1;
    scale.y = 1;
    texture = nullptr;
    associated.angleDeg = 0;
    currentFrame = 0;
    timeElapsed = 0;
    this->parallaxvalue = 1;
}

Sprite::Sprite(GameObject& associated,std::string file,int frameCount,float frameTime,float secondsToSelfDestruct,bool repeat,bool singularTexture) :
    Sprite(associated,frameCount,frameTime,secondsToSelfDestruct,repeat,singularTexture){

    Open(file);
}

Sprite::~Sprite(){
    delete selfDestructCount;
    if(singularTexture){
    }
}

void Sprite::Open(std::string file){
    if(singularTexture){
        SDL_Texture *textureSDL = IMG_LoadTexture(Game::GetInstance().GetRenderer(), file.c_str());
        std::shared_ptr<SDL_Texture> textptr(textureSDL,Resources::DeleteImage);
        texture = textptr;
    }else{
        texture = Resources::GetImage(file);
    }
    if(texture == nullptr){
        std::cout << "Error Loading Image: " << SDL_GetError() << std::endl;
        return;
    }else{
        SetFrame(0);
        SDL_QueryTexture(texture.get(),nullptr,nullptr, &width, &height);
        this->associated.box.w = (width/frameCount) * scale.x;
        this->associated.box.h = (height) * scale.y;
        SetClip(0,0,width/frameCount,height);
        
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
        associated.box.w = width * scaleX;
    }
    if(scaleY != 0){
        scale.y = scaleY;
        associated.box.y = associated.box.y + (associated.box.h - (associated.box.h*scaleY));
        associated.box.h = height * scaleY;
    }
}

void Sprite::SetFrame(int frame){
    currentFrame = frame;
    SetClip((width/frameCount)*currentFrame,0,width/frameCount,height);
}

void Sprite::SetFrameCount(int frameCount){
    currentFrame = 0;
    this->frameCount = frameCount;
}

void Sprite::SetFrameTime(float frameTime){
    this->frameTime = frameTime;
}

void Sprite::SetAlpha(int alpha){
    SDL_SetTextureAlphaMod(texture.get(),alpha);
}

void Sprite::SetParallax(float value){
    parallaxvalue = value;
}

void Sprite::Update(float dt){
    if((repeat) || (!repeat && (currentFrame < frameCount))){
        timeElapsed += dt;
        if(timeElapsed >= frameTime ){
            timeElapsed = 0;
            currentFrame += 1;
            if(currentFrame >= frameCount){
                if(repeat){
                    currentFrame = 0;
                }else{
                    currentFrame -= 1;
                }
            }
            SetClip((width/frameCount)*currentFrame,0,width/frameCount,height);
        }
    }
    if(secondsToSelfDestruct > 0){
        selfDestructCount->Update(dt);
        if(selfDestructCount->Get() > secondsToSelfDestruct){
            associated.RequestDelete();
        }
    }
}

void Sprite::Render(){
    Render(this->associated.box.x - Camera::pos.x * parallaxvalue,this->associated.box.y - Camera::pos.y * parallaxvalue);
}

void Sprite::Render(int x,int y){
    SDL_Rect dst_rect;
    dst_rect.x = x;
    dst_rect.y = y;
    dst_rect.w = clip_rect.w * scale.x;
    dst_rect.h = clip_rect.h * scale.y;
    if(flip){
        SDL_RenderCopyEx(Game::GetInstance().GetRenderer(),texture.get(),&clip_rect,&dst_rect,associated.angleDeg,nullptr,SDL_FLIP_HORIZONTAL);
    }else{
        SDL_RenderCopyEx(Game::GetInstance().GetRenderer(),texture.get(),&clip_rect,&dst_rect,associated.angleDeg,nullptr,SDL_FLIP_NONE);
    }
}

int Sprite::GetWidth(){
    return (width * scale.x) / frameCount;
}

int Sprite::GetHeight(){
    return height * scale.y;
}

void Sprite::Flip(){
    if(flip){
        flip = false;
    }else{
        flip = true;
    }
}

bool Sprite::IsFlipped(){
    return flip;
}

bool Sprite::IsOver(){
    if(!repeat && ((frameCount - 1) == currentFrame)){
        return true;
    }else{
        return false;
    }
}

void Sprite::SetRepeat(bool repeat){
    this->repeat = repeat;
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