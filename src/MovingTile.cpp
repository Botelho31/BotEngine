#include "../include/MovingTile.h"

MovingTile::MovingTile(GameObject& associated) : Component(associated){
    this->physics = new Physics(&associated,&speed,true);
    physics->SetCollider(1.0,4.5,0,-100);
    this->tilesprite = new Sprite(associated,"assets/img/penguin.png");
    this->deltamov = Vec2(0,0);
    associated.AddComponent(tilesprite);   
    speed.y = 200;
    speed.x = 0;
}

MovingTile::~MovingTile(){
    delete physics;
    tilesprite = nullptr;
}

void MovingTile::Update(float dt){
    physics->Update(physics->GetCollider()->box);
    this->deltamov.x = physics->PerformXMovement(dt);
    this->deltamov.y = physics->PerformYMovement(dt);
    
    if(physics->distright <= 0){
        speed.x = -200;
    }
    if(physics->distleft <= 0){
        speed.x = 200;
    }
    if(physics->distceiling <= 0){
        speed.y = 200;
    }
    if(physics->distground <= 0){
        speed.y = -200;
    }
}

void MovingTile::Render(){

}

void MovingTile::NotifyCollision(GameObject& other){
    // Component *component = other.GetComponent("Collider");
    // if(component){
    //     Collider *collider = dynamic_cast<Collider*>(component);
    //     float difground = associated.box.y - (collider->box.y + collider->box.h);
    //     std::cout << difground << std::endl;
    //     std::cout << deltamov.y << std::endl;
    //     if(difground < 0){
    //         other.box.y += difground;
    //     }
    // }
    // Component *component = other.GetComponent("Player");
    // component->KeepStill(true);

    other.box.y += deltamov.y;
    other.box.x += deltamov.x;
}

bool MovingTile::Is(std::string type){
    if(type == "MovingTile"){
        return true;
    }else{
        return false;
    }
}