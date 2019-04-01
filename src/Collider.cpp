#include "Collider.h"

Collider::Collider(GameObject& associated,Vec2 scale,Vec2 offset) : 
    Component(associated),scale(scale),offset(offset){
}

void Collider::Update(float dt){
    box = Rect(associated.box.x,associated.box.y,associated.box.w,associated.box.h);
    box.w *= scale.x;
    box.h *= scale.y;
    box.x = associated.box.x + associated.box.w/2 - box.w/2 + offset.x;
    box.y = associated.box.y + associated.box.h/2 - box.h/2 + offset.y;
    Vec2 origin = Vec2(-box.w/2,-box.h/2).GetRotated(associated.angleDeg * 180 / PI); 
    box.Transform(origin.x + box.x,origin.y + box.y);
}

void Collider::Render(){

}

bool Collider::Is(std::string type){
    if(type == "Collider"){
        return true;
    }else{
        return false;
    }
}

void Collider::SetScale(Vec2 scale){
    this->scale.Transform(scale);
}

void Collider::SetOffSet(Vec2 offset){
    this->offset.Transform(offset);
}