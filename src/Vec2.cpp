#include "../include/Vec2.h"

Vec2::Vec2(){
    x = 0;
    y = 0;
}

Vec2::Vec2(float x,float y){
    this->x = x;
    this->y = y;
}

Vec2& Vec2::GetRotated(float R){
    float x,y;
    x = this->x;
    y = this->y;
    this->x = (x * cos(R)) - (y * sin(R)); 
    this->y = (y * cos(R)) + (x * sin(R));
    return *this;
}