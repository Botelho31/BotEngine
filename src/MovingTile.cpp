#include "../include/MovingTile.h"

MovingTile::MovingTile(GameObject& associated,float speed,Vec2 start,Vec2 dest) : Component(associated){
    this->tilesprite = new Sprite(associated,"assets/img/penguin.png");
    associated.AddComponent(tilesprite);
    this->associated.box.x = start.x - associated.box.w/2;
    this->associated.box.y = start.y - associated.box.h/2;

    this->physics = new Physics(&associated,&this->speed,true);
    physics->SetCollider(1.1,1.0,0,-50);

    this->deltamov = Vec2(0,0);
    this->constspeed = speed;
    this->speed =  Vec2(speed,speed);
    this->start = start;
    this->dest = dest;
    this->going = true;
}

MovingTile::~MovingTile(){
    delete physics;
    tilesprite = nullptr;
}

void MovingTile::Update(float dt){
    physics->Update(physics->GetCollider()->box);
    
    if(going){
        deltamov = physics->Follow(dest,constspeed,dt);
    }else{
        deltamov = physics->Follow(start,constspeed,dt);
    }

    if(deltamov == Vec2(0,0)){
        if(going){
            going = false;
        }else{
            going = true;
        }
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