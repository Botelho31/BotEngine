#include "../include/Spike.h"
#include "../include/Collider.h"

Spike::Spike(GameObject& associated,Vec2 knockback) : Component(associated){
    this->knockback = knockback;
    this->damage = 300;
    Collider *collider = new Collider(associated);
    associated.AddComponent(collider);
}

Spike::~Spike(){

}

void Spike::Update(float dt){

}

void Spike::Render(){

}

bool Spike::Is(std::string type){
    if(type == "Spike"){
        return true;
    }else{
        return false;
    }
}

Vec2 Spike::GetKnockback(){
    return knockback;
}

int Spike::GetDamage(){
    return damage;
}