#include "../include/ClosingDoor.h"
#include "../include/TileMap.h"
#include "../include/TileCollider.h"


ClosingDoor::ClosingDoor(GameObject& associated,std::string doorspritefile,Vec2 opened,Vec2 closed,doorState initialstate) : Component(associated){
    this->opened = opened;
    this->closed = closed;
    this->state = STANDBY;

    Sprite *doorsprite = new Sprite(associated,doorspritefile);
    associated.AddComponent(doorsprite);
    TileCollider* tilecollider = new TileCollider(associated,associated.box,true);
    std::weak_ptr<Component> weakptr = associated.AddComponent(tilecollider);
    TileMap::tiles.push_back(weakptr);

    this->physics =  new Physics(associated,&speed,true);
    if(initialstate == OPENING){
        this->associated.box.SetCenter(opened);
    }else{
        this->associated.box.SetCenter(closed);
    }
}

ClosingDoor::~ClosingDoor(){

}

void ClosingDoor::Update(float dt){
    switch(state){
            case OPENING:
                Vec2 delta = physics->Follow(opened,100,dt);
                if(delta == Vec2(0,0)){
                    state = STANDBY;
                }
                break;
            case CLOSING:
                Vec2 delta = physics->Follow(closed,100,dt);
                if(delta == Vec2(0,0)){
                    state = STANDBY;
                }
                break;
            case STANDBY:
                break;
            default:
                break;
        }
}

void ClosingDoor::Render(){

}

bool ClosingDoor::Is(std::string type){
    if(type == "ClosingDoor"){
        return true;
    }else{
        return false;
    }
}

void ClosingDoor::NotifyCollision(GameObject& other){

}

void ClosingDoor::Close(){
    state = CLOSING;
}

void ClosingDoor::Open(){
    state = OPENING;
}