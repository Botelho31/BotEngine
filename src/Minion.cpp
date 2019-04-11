#include "../include/Minion.h"
#include "../include/Collider.h"

Minion::Minion(GameObject& associated) : Component(associated){
    speed.x = 0;
    maxspeed = 600;
    aspeed = 700;
    despeed = 800;

    speed.y = 0;
    gravspeed = 2000;
    hittheground = new Timer();

    hp = 150;

    idletimer = new Timer();
    idle = false;

    Sprite *minion =  new Sprite(associated,"assets/img/miniontest.png");
    this->minionsprite = minion;
    Collider *collider = new Collider(associated);
    associated.AddComponent(collider);
    associated.AddComponent(minion);
    // SetCollider(0.6,1);
}

Minion::~Minion(){

}

void Minion::Start(){

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

void Minion::NotifyCollision(GameObject& other){

}
