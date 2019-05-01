#include "../include/MovingTile.h"

MovingTile::MovingTile(GameObject& associated) : Component(associated){
    this->physics = new Physics(&associated,&speed,true);
    this->tilesprite = new Sprite(associated,"assets/img/penguin.png");
    associated.AddComponent(tilesprite);
}

void MovingTile::Update(float dt){
    speed.x = 200;
    physics->Update(physics->GetCollider()->box);
    physics->PerformXMovement(dt);
}

void MovingTile::Render(){

}

void MovingTile::NotifyCollision(GameObject& other){
}

bool MovingTile::Is(std::string type){
    if(type == "MovingTile"){
        return true;
    }else{
        return false;
    }
}