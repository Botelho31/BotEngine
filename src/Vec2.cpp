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
    this->y = (x * sin(R)) + (y * cos(R));
    return *this;
}

void Vec2::Transform(float x,float y){
    this->x = x;
    this->y = y;
}

void Vec2::Transform(Vec2 pos){
    this->x = pos.x;
    this->y = pos.y;
}

Vec2 Vec2::Added(float x,float y){
    return Vec2(this->x + x,this->y + y);
}

float Vec2::GetAngle(float x,float y,float offsetX,float offsetY){
    float boxX = this->x + offsetX;
    float boxY = this->y + offsetY;
    float difX = x - boxX;
    float difY = boxY - y;
    return -atan2(difY,difX);
}

float Vec2::GetAngle(Vec2 point,float offsetX,float offsetY){
    float boxX = this->x + offsetX;
    float boxY = this->y + offsetY;
    float difX = point.x - boxX;
    float difY = boxY - point.y;
    return -atan2(difY,difX);
}

float Vec2::GetDistance(float x,float y){
    return sqrt(pow(this->x - x,2) + pow(this->y - y,2));
}