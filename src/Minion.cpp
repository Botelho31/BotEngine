#include "../include/Minion.h"
#include "../include/Sprite.h"
#include "../include/Vec2.h"
#include "../include/Camera.h"
#include "../include/Bullet.h"
#include "../include/Collider.h"
#include "../include/Alien.h"
#include "../include/Sound.h"

Minion::Minion(GameObject& associated,std::weak_ptr<GameObject> alienCenter,float arcOffsetDeg) : 
    Component(associated),alienCenter(alienCenter), arc(arcOffsetDeg){
    hp = 15;
    Sprite* minion = new Sprite(associated,"assets/img/minion.png");
    associated.box.Transform(0,0);
    Collider *collider = new Collider(associated);
    associated.AddComponent(collider);
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
    if(hp <= 0){
        GameObject *explosionObj = new GameObject();
        Sprite *explosion = new Sprite(*explosionObj,"assets/img/miniondeath.png",4,0.2,0.8);
        explosionObj->box.Transform(associated.box.x + associated.box.w/2 - explosionObj->box.w/2,associated.box.y + associated.box.h/2 - explosionObj->box.h/2);
        explosionObj->angleDeg = associated.angleDeg;
        Sound *sound =  new Sound(*explosionObj,"assets/audio/boom.wav");
        sound->Play(1);
        explosionObj->AddComponent(explosion);
        explosionObj->AddComponent(sound);
        Game::GetInstance().GetCurrentState().AddObject(explosionObj);
        associated.RequestDelete();
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
    GameObject *bulletObj = new GameObject();
    Bullet *bullet = new Bullet(*bulletObj,angle,100,100,1000,"assets/img/minionbullet2.png",3,0.5,true);
    bulletObj->box.Transform(associated.box.x + associated.box.w/2 - bulletObj->box.w/2,associated.box.y + associated.box.h/2 - bulletObj->box.h); 
    bulletObj->AddComponent(bullet);
    Game::GetInstance().GetCurrentState().AddObject(bulletObj);
}

void Minion::NotifyCollision(GameObject& other){
    Component *component = other.GetComponent("Bullet");    
    if(component){
        Bullet *bullet = dynamic_cast<Bullet*>(component);
        if(!bullet->targetsPlayer){
            hp -= bullet->GetDamage();
        }
    }
}