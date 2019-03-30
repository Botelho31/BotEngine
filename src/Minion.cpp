#include "../include/Minion.h"
#include "../include/Sprite.h"
#include "../include/Vec2.h"
#include "../include/Camera.h"
#include "../include/Bullet.h"

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
        arc += (PI/8) * dt;
        associated.angleDeg = arc * 180 / PI;
        Vec2 vector = Vec2(110,0).GetRotated(arc) + Vec2(alien->box.x + alien->box.w/2,alien->box.y + alien->box.h/2);
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
    float angle = associated.box.GetAngle(target.x,target.y,associated.box.w/2,associated.box.h/2);
    GameObject *bulletObj = new GameObject(&associated.GetState());
    Bullet *bullet = new Bullet(*bulletObj,angle,100,100,1000,"assets/img/minionbullet2.png",3,0.5);
    bulletObj->box.Transform(associated.box.x + associated.box.w/2 - bulletObj->box.w/2,associated.box.y + associated.box.h/2 - bulletObj->box.h); 
    bulletObj->AddComponent(bullet);
    associated.GetState().AddObject(bulletObj);
}