#include "../include/Rect.h"

Rect::Rect(){
    w = 0;
    h = 0;
    x = 0;
    y = 0;
}

Rect::Rect(float x,float y,float w,float h){
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
}

bool Rect::Contains(float x,float y){
    if(( (this->x <= x) && (x <= (this->x + this->w)) ) && ( (this->y <= y) && (y <= (this->y + this->h)) )){
        return true;
    }else{
        return false;
    }
}
