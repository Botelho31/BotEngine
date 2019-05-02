#include "../include/MovingTile.h"

MovingTile::MovingTile(GameObject& associated) : Component(associated){
    this->physics = new Physics(&associated,&speed,true);
    physics->SetCollider(1.2,1.2);
    this->tilesprite = new Sprite(associated,"assets/img/penguin.png");
    this->deltamov = Vec2(0,0);
    associated.AddComponent(tilesprite);
}

MovingTile::~MovingTile(){
    delete physics;
    tilesprite = nullptr;
}

void MovingTile::Update(float dt){
    speed.x = 200;
    physics->Update(physics->GetCollider()->box);
    this->deltamov.x = physics->PerformXMovement(dt);
    std::cout << speed.x << " " << physics->distright << std::endl;
    if((physics->distright <= 0) || (physics->distleft <= 0)){
        speed.x = -200;
    }
    std::cout << speed.x << " " << physics->distright << std::endl;
}

void MovingTile::Render(){

}

void MovingTile::NotifyCollision(GameObject& other){
    other.box.x += deltamov.x;
}

bool MovingTile::Is(std::string type){
    if(type == "MovingTile"){
        return true;
    }else{
        return false;
    }
}