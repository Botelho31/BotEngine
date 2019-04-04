#include "../include/Text.h"
#include "../include/Camera.h"
#include "../include/Resources.h"

Text::Text(GameObject& associated,std::string fontFile,int fontSize,TextStyle style,std::string text,SDL_Color color) :
    Component(associated){
        this->fontFile = fontFile;
        this->fontSize = fontSize;
        this->style = style;
        this->text = text;
        this->color = color;
        this->texture = nullptr;
        RemakeTexture();
}

Text::~Text(){
    if(texture){
        SDL_DestroyTexture(texture);
    }
}

void Text::Update(float dt){

}

void Text::Render(){
    if(texture){
        SDL_Rect clipRect;
        clipRect.x = 0;
        clipRect.y = 0;
        clipRect.w = associated.box.w;
        clipRect.h = associated.box.h;
        SDL_Rect dstRect;
        dstRect.x = associated.box.x - Camera::pos.x;
        dstRect.y = associated.box.y - Camera::pos.y;
        dstRect.w = clipRect.w;
        dstRect.h = clipRect.h;
        SDL_RenderCopyEx(Game::GetInstance().GetRenderer(),texture,&clipRect,&dstRect,associated.angleDeg,nullptr,SDL_FLIP_NONE);
    }
}

void Text::SetText(std::string text){
    this->text = text;
    RemakeTexture();
}

void Text::SetColor(SDL_Color color){
    this->color = color;
    RemakeTexture();
}

void Text::SetStyle(TextStyle style){
    this->style = style;
    RemakeTexture();
}

void Text::SetFontFile(std::string fontFile){
    this->fontFile = fontFile;
    RemakeTexture();
}

void Text::SetFontSize(int fontSize){
    this->fontSize = fontSize;
    RemakeTexture();
}

void Text::RemakeTexture(){
    if(texture){
        SDL_DestroyTexture(texture);
    }
    font = Resources::GetFont(fontFile,fontSize);
    if(font){
        SDL_Surface *surface;
        switch(style){
            case SOLID:
                surface = TTF_RenderText_Solid(font.get(),text.c_str(),color);
                break;
            case SHADED:
                surface = TTF_RenderText_Shaded(font.get(),text.c_str(),color,color);
                   break;
            case BLENDED:
                surface = TTF_RenderText_Blended(font.get(),text.c_str(),color);
                break;
            default:
                std::cout << "No style found" << std::endl;
        }
       texture = SDL_CreateTextureFromSurface(Game::GetInstance().GetRenderer(),surface);
        SDL_FreeSurface(surface);
        int width,height;
        SDL_QueryTexture(texture,nullptr,nullptr, &width, &height);
        associated.box.w = width;
        associated.box.h = height;
    }
    
}

bool Text::Is(std::string type){
    if(type == "Text"){
        return true;
    }else{
        return false;
    }
}