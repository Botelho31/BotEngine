#include "../include/HitBox.h"


HitBox::HitBox(GameObject& associated,Rect hitbox,std::weak_ptr<GameObject> owner,double angledeg,int damage,float secondsToSelfDestruct,float damageCooldown,bool disconnected,bool hitPlayer,bool hitEnemy,Vec2 knockback,Component *component) : 
    Component(associated),secondsToSelfDestruct(secondsToSelfDestruct),Move(NULL),owner(owner),damageCooldown(damageCooldown),hitPlayer(hitPlayer),hitEnemy(hitEnemy),disconnected(disconnected),damage(damage),knockback(knockback),component(component){
    associated.box = hitbox;
    associated.angleDeg = angledeg;
    this->selfDestruct = new Timer();
    Collider *collider = new Collider(associated);
    associated.AddComponent(collider);
}

HitBox::~HitBox(){
    component = nullptr;
    delete selfDestruct;
}

void HitBox::SetFunction( void(*NewFunc)(GameObject&,GameObject&,float) ){
    this->Move = NewFunc;
}

void HitBox::Update(float dt){
    if(secondsToSelfDestruct > 0){
        selfDestruct->Update(dt);
        damageCooldown -= dt;
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
    if(!disconnected){
        if(!owner.expired()){
            if(component){
                component->NotifyCollision(other);
            }
        }
    }
}

bool HitBox::Is(std::string type){
    if(type == "HitBox"){
        return true;
    }else{
        return false;
    }
}

Collider* HitBox::GetCollider(){
    Component *component = associated.GetComponent("Collider");
    if(component){
        Collider *collider = dynamic_cast<Collider*>(component);
        return collider;
    }else{
        return nullptr;
    }
}

std::shared_ptr<GameObject> HitBox::GetOwner(){
    if(!owner.expired()){
        return owner.lock();
    }else{
        return nullptr;
    }
}

Vec2 HitBox::GetKnockBack(){
    return this->knockback;
}

bool HitBox::HitPlayer(){
    return hitPlayer;
}

bool HitBox::HitEnemy(){
    return hitEnemy;
}

float HitBox::GetDamageCooldown(){
    return damageCooldown;
}

int HitBox::GetDamage(){
    return damage;
}

void HitBox::SetSelfDestruct(float secondsToSelfDestruct){
    this->secondsToSelfDestruct = secondsToSelfDestruct;
}

void HitBox::SetDamageValues(int damage,float damageCooldown,Vec2 knockback){
    this->damage = damage;
    this->damageCooldown = damageCooldown;
    this->knockback = knockback;
}

void HitBox::SetHit(bool HitPlayer,bool HitEnemy){
    this->hitPlayer = HitPlayer;
    this->hitEnemy = HitEnemy;
}

