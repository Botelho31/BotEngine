#include "../include/HitBox.h"

HitBox::HitBox(GameObject& associated,Rect hitbox,double angledeg,bool HitPlayer,bool HitEnemy) : 
    Component(associated),HitPlayer(HitPlayer),HitEnemy(HitEnemy){
        associated.box = hitbox;
        associated.angleDeg = angledeg;
        this->collider = new Collider(associated);
        associated.AddComponent(collider);
}

HitBox::~HitBox(){
    collider = nullptr;
}

void HitBox::Update(float dt){

}

void HitBox::Render(){

}

bool HitBox::Is(std::string type){
    if(type == "HitBox"){
        return true;
    }else{
        return false;
    }
}

