#include "../include/HitBox.h"

HitBox::HitBox(GameObject& associated,std::weak_ptr<GameObject> owner,Rect hitbox,bool HitPlayer,float secondsToSelfDestruct,bool HitEnemy,double angledeg) : 
    Component(associated),HitPlayer(HitPlayer),HitEnemy(HitEnemy),Move(NULL),owner(owner){
        associated.box = hitbox;
        associated.angleDeg = angledeg;
        this->time = 0;
        this->secondsToSelfDestruct = secondsToSelfDestruct;
        this->collider = new Collider(associated);
        associated.AddComponent(collider);
}

HitBox::~HitBox(){
    collider = nullptr;
}

void HitBox::SetFunction( void(*NewFunc)(GameObject&,GameObject&,float) ){
    this->Move = NewFunc;
}

void HitBox::Update(float dt){
    if(secondsToSelfDestruct > 0){
        time += dt;
        if(time >= secondsToSelfDestruct){
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

