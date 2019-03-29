#include "../include/Minion.h"
#include "../include/Sprite.h"
#include "../include/Vec2.h"

Minion::Minion(GameObject& associated,std::weak_ptr<GameObject> alienCenter,float arcOffsetDeg) : 
    Component(associated),alienCenter(alienCenter){
    Sprite* minion = new Sprite(associated,"assets/img/minion.png");
    associated.box.Transform(0,0);
    associated.AddComponent(minion);
}

Minion::~Minion(){
    
}

void Minion::Update(float dt){
    std::shared_ptr<GameObject> alien = alienCenter.lock();
    if(alien){
        Vec2 vector = Vec2(200,0);
        vector.GetRotated(arc + PI/20);
        Vec2 final = vector + Vec2(alien->box.x,alien->box.y);
        Component *component = associated.GetComponent("Sprite");
        if(component != nullptr){
            Sprite *minion = dynamic_cast<Sprite*>(component);
            associated.box.Transform(vector.x - (minion->GetWidth()/2),vector.y - (minion->GetHeight()/2));
        }else{
            associated.box.Transform(vector.x,vector.y);
        }
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