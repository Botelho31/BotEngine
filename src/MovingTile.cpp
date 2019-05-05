#include "../include/MovingTile.h"

MovingTile::MovingTile(GameObject& associated,float constSpeed,Vec2 start,Vec2 dest) : Component(associated){

    this->physics = new Physics(&associated,&speed,true);
    physics->SetCollider(1.0,4.5,0,-100);

    this->tilesprite = new Sprite(associated,"assets/img/penguin.png");
    associated.AddComponent(tilesprite);
    this->associated.box.x = start.x - associated.box.w/2;
    this->associated.box.y = start.y - associated.box.h/2;

    this->deltamov = Vec2(0,0);
    this->speed =  Vec2(constSpeed,constSpeed);
    this->start = start;
    this->dest = dest;
}

MovingTile::~MovingTile(){
    delete physics;
    tilesprite = nullptr;
}

void MovingTile::Update(float dt){
    physics->Update(physics->GetCollider()->box);
    
    deltamov = physics->Follow(dest,dt);


    // if(physics->distright <= 0){
    //     speed.x = -constSpeed;
    // }
    // if(physics->distleft <= 0){
    //     speed.x = constSpeed;
    // }
    // if(physics->distceiling <= 0){
    //     speed.y = constSpeed;
    // }
    // if(physics->distground <= 0){
    //     speed.y = -constSpeed;
    // }
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