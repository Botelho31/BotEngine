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
    this->x = (this->x * cos(R)) - (this->y * sin(R)); 
    this->y = (this->y * cos(R)) + (this->x * sin(R));
    return *this;
}