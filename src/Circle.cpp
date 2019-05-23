#include "../include/Circle.h"

Circle::Circle(){
    x = 0;
    y = 0;
    radius = 0;
}
Circle::Circle(int x,int y,int radius){
    this->x = x;
    this->y = y;
    this->radius = radius;
}
Circle::Circle(Vec2 center,int radius){
    this->x = center.x;
    this->y = center.y;
    this->radius = radius;
}

bool Circle::IsInside(int x,int y){
    if(pow((x - this->x),2) + pow((y - this->y),2) <= pow(radius,2)){
        return true;
    }else{
        return false;
    }
}
bool Circle::IsInside(Vec2 pos){
    int x = pos.x;
    int y = pos.y;
    if(pow((x - this->x),2) + pow((y - this->y),2) <= pow(radius,2)){
        return true;
    }else{
        return false;
    }
}
void Circle::Transform(int x,int y){
    this->x = x;
    this->y = y;
}
void Circle::Transform(Vec2 pos){
    this->x = pos.x;
    this->y = pos.y;
}
float Circle::GetDistanceFromCenter(float x,float y){
    return sqrt(pow(this->x - x,2) + pow(this->y - y,2));
}