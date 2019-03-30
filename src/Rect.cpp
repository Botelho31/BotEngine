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

void Rect::Transform(float x,float y){
    this->x = x;
    this->y = y;
}

float Rect::Magnitude(){
    return sqrt(x*x + y*y);
}

float Rect::MultiplyVector(float x,float y){
    return (this->x * this->y) + (x*y);
}

bool Rect::Follow(float x,float y, float speedX,float speedY,float dt){
    bool DoneX = false;
    bool DoneY = false;

    if(this->x == x){
        DoneX = true;
    }
    else if(this->x < x){
        this->x += speedX * dt;
        if(this->x > x){
            this->x = x;
        }
    }else{
        this->x -= speedX * dt;
        if(this->x < x){
            this->x = x;
        }
    }

    if(this->y == y){
        DoneY = true;
    }
    else if(this->y < y){
        this->y += speedY * dt;
        if(this->y > y){
            this->y = y;
            DoneY = true;
        }
    }else{
        this->y -= speedY * dt;
        if(this->y < y){
            this->y = y;
            DoneY = true;
        }
    }
    return (DoneX && DoneY);
}
