#include "../include/Spike.h"
#include "../include/Collider.h"
#include "../include/Camera.h"

Spike::Spike(GameObject& associated,Vec2 knockback) : Component(associated){
    this->knockback = knockback;
    this->damage = 300;
    Collider *collider = new Collider(associated);
    associated.AddComponent(collider);
    GameObject *ownerObj = new GameObject();
    std::vector<std::string> tilesprites;
    tilesprites.push_back("assets/img/Spiketest.png");
    spiketileset = new TileSet(ownerObj,50,150,tilesprites);
}

Spike::~Spike(){

}

void Spike::Update(float dt){

}

void Spike::Render(){
    int currentwidth = 0;
    int currentIndex = 1;
    int maxIndex = 4;
    for(int i = 0;i < (associated.box.w/spiketileset->GetTileWidth());i++){
        if(i == 0){
            spiketileset->RenderTile(0,associated.box.x + currentwidth - Camera::pos.x,associated.box.y - Camera::pos.y);
        }else if((i + 1) == (associated.box.w/spiketileset->GetTileWidth())){
            spiketileset->RenderTile(4,associated.box.x + currentwidth - Camera::pos.x,associated.box.y - Camera::pos.y);
        }else{
            spiketileset->RenderTile(2,associated.box.x + currentwidth - Camera::pos.x,associated.box.y - Camera::pos.y);
        }
        currentwidth += spiketileset->GetTileWidth();
    }
}

bool Spike::Is(std::string type){
    if(type == "Spike"){
        return true;
    }else{
        return false;
    }
}

Vec2 Spike::GetKnockback(){
    return knockback;
}

int Spike::GetDamage(){
    return damage;
}