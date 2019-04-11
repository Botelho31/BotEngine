#include "../include/HitBox.h"

HitBox::HitBox(GameObject& associated,std::weak_ptr<GameObject> owner,Rect hitbox,int damage,float damageCooldown,bool HitPlayer,bool HitEnemy,float secondsToSelfDestruct,double angledeg) : 
    Component(associated),HitPlayer(HitPlayer),HitEnemy(HitEnemy),Move(NULL),owner(owner){
        associated.box = hitbox;
        associated.angleDeg = angledeg;
        this->damage = damage;
        this->damageCooldown = damageCooldown;
        this->selfDestruct = new Timer();
        this->secondsToSelfDestruct = secondsToSelfDestruct;
        Collider *collider = new Collider(associated);
        associated.AddComponent(collider);
}

HitBox::~HitBox(){
}

void HitBox::SetFunction( void(*NewFunc)(GameObject&,GameObject&,float) ){
    this->Move = NewFunc;
}

void HitBox::Update(float dt){
    if(secondsToSelfDestruct > 0){
        selfDestruct->Update(dt);
        if(selfDestruct->Get() >= secondsToSelfDestruct){
            associated.RequestDelete();
        }
    }
    if(!owner.expired()){
        if(Move != NULL){
            Move(associated,*owner.lock().get(),dt);
        }
    }else{
        associated.RequestDelete();
    }

}

void HitBox::Render(){

}

void HitBox::NotifyCollision(GameObject& other){

}

bool HitBox::Is(std::string type){
    if(type == "HitBox"){
        return true;
    }else{
        return false;
    }
}

