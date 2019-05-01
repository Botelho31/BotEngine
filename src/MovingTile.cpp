#include "../include/MovingTile.h"

MovingTile::MovingTile(GameObject& associated) : Component(associated){
    this->physics = new Physics(&associated,&speed);
    this->tilesprite = new Sprite(associated,"assets/img/penguin.png");
    associated.AddComponent(tilesprite);
}

void MovingTile::Update(float dt){

}

void MovingTile::Render(){

}

bool MovingTile::Is(std::string type){
    if(type == "MovingTile"){
        return true;
    }else{
        return false;
    }
}