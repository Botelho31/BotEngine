#include "../include/Minion.h"
#include "../include/Sprite.h"
#include "../include/Vec2.h"
#include "../include/Camera.h"

Minion::Minion(GameObject& associated,std::weak_ptr<GameObject> alienCenter,float arcOffsetDeg) : 
    Component(associated),alienCenter(alienCenter), arc(arcOffsetDeg){
    Sprite* minion = new Sprite(associated,"assets/img/minion.png");
    associated.box.Transform(0,0);
    associated.AddComponent(minion);
}

Minion::~Minion(){
    
}

void Minion::Update(float dt){
    std::shared_ptr<GameObject> alien = alienCenter.lock();
    if(alien){
        arc += PI/20;
        Vec2 vector = Vec2(200,0).GetRotated(arc) + Vec2(alien->box.x + alien->box.w/2,alien->box.y + alien->box.h/2);
        associated.box.Transform(vector.x - associated.box.w/2,vector.y - associated.box.h/2);
    }else{
        associated.RequestDelete();
    }
}

void Minion::Render(){

}

bool Minion::Is(std::string type){
    if(type == "Minion"){
        return true;
    }else{
        return false;
    }
}

void Minion::Shoot(Vec2 target){

}