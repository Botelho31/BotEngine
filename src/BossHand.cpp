#include "../include/BossHand.h"
#include "../include/Collider.h"
#include "../include/TileMap.h"

BossHand::BossHand(GameObject& associated,std::weak_ptr<GameObject> boss,Vec2 pos) : Component(associated){
    this->boss = boss;
    associated.box.w = 400;
    associated.box.h = 300;
    associated.box.Transform(pos.x,pos.y);
    TileCollider *tilecollider = new TileCollider(associated,associated.box,true);
    std::weak_ptr<Component> weakptr = associated.AddComponent(tilecollider);
    if(!weakptr.expired()){
        TileMap::tiles.push_back(weakptr);
    }

    Collider *collider = new Collider(associated);
    associated.AddComponent(collider);
    collider->Update(0);
}
BossHand::~BossHand(){

}
void BossHand::Update(float dt){

}
void BossHand::Render(){

}
bool BossHand::Is(std::string type){
    if(type == "BossHand"){
        return true;
    }else{
        return false;
    }
}

void BossHand::NotifyCollision(GameObject& other){
    if(!boss.expired()){
        boss.lock()->NotifyCollision(other);
    }
}