#include "../include/Vec2.h"

Vec2::Vec2(){
    x = 0;
    y = 0;
}

Vec2::Vec2(float x,float y){
    this->x = x;
    this->y = y;
}

float Vec2::Magnitude(){
    return sqrt(x*x + y*y);
}

Vec2& Vec2::GetRotated(float R){
    float x,y;
    x = this->x;
    y = this->y;
    this->x = (x * cos(R)) - (y * sin(R)); 
    this->y = (y * cos(R)) + (x * sin(R));
    return *this;
}

bool Vec2::Follow(float x,float y, float speedX,float speedY,float dt){
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