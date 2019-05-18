#include "../include/Rect.h"
#include "../include/Vec2.h"

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

bool Rect::Contains(Vec2 point){
    if(( (this->x <= point.x) && (point.x <= (this->x + this->w)) ) && ( (this->y <= point.y) && (point.y <= (this->y + this->h)) )){
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

Rect Rect::Added(float x,float y,float w,float h){
    return Rect(this->x + x,this->y + y,this->w + w,this->h + h);
}

Rect Rect::Added(Rect added){
    return Rect(x + added.x,y + added.y,w + added.w,h + added.y);
}

float Rect::GetDistance(float x,float y){
    return sqrt(pow(this->x - x,2) + pow(this->y - y,2));
}

Vec2 Rect::GetOrigin(){
    return Vec2(x,y);
}

Vec2 Rect::GetCenter(){
    return Vec2(x + w/2,y + h/2);
}

Vec2 Rect::GetMirroredPoint(Vec2 point,bool horizontal,bool vertical){
    if(horizontal){
        float difX = x - point.x;
        float difXW = (x + w)  - point.x;
        float difpointX = difX + difXW;
        point.x += difpointX;
    }
    if(vertical){
        float difY = y - point.y;
        float difYH = (y + h) - point.y;
        float difpointY = difY + difYH;
        point.y += difpointY;
    }
    return point;
}

void Rect::SetCenter(Vec2 center){
    x = center.x - w/2;
    y = center.y - h/2;
}

float Rect::GetAngle(float x,float y,float offsetX,float offsetY){
    float boxX = this->x + offsetX;
    float boxY = this->y + offsetY;
    float difX = x - boxX;
    float difY = boxY - y;
    return -atan2(difY,difX);
}
