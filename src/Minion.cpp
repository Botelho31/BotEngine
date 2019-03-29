#include "../include/Minion.h"

Minion::Minion(GameObject& associated,std::weak_ptr<GameObject> alienCenter,float arcOffsetDeg) : 
Component(associated),alienCenter(alienCenter){
}

Minion::~Minion(){
    
}

void Minion::Update(float dt){

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