#include "../include/Spike.h"
#include "../include/Collider.h"
#include "../include/Camera.h"

Spike::Spike(GameObject& associated,Vec2 knockback,bool flipped) : Component(associated){
    this->knockback = knockback;
    this->damage = 300;
    Collider *collider = new Collider(associated);
    associated.AddComponent(collider);
    this->flipped = flipped;

    GameObject *ownerObj = new GameObject();
    std::vector<std::string> tilesprites;
    tilesprites.push_back("assets/img/Spiketest1.png");
    tilesprites.push_back("assets/img/Spiketest2.png");
    spiketileset = new TileSet(ownerObj,50,150,tilesprites);

    GameObject *ownerObj2 = new GameObject();
    std::vector<std::string> tilesprites2;
    tilesprites2.push_back("assets/img/Spiketest2.png");
    tilesprites2.push_back("assets/img/Spiketest22.png");
    spiketileset2 = new TileSet(ownerObj2,150,50,tilesprites2);
}

Spike::~Spike(){
    delete spiketileset2;
    delete spiketileset;
}

void Spike::Update(float dt){

}

void Spike::Render(){
    int offset = 0;
    if(flipped){
        offset = 5;
    }
    if(associated.box.h == spiketileset->GetTileHeight()){
        int currentwidth = 0;
        int currentIndex = 1;
        int maxIndex = 4;
        for(int i = 0;i < (associated.box.w/spiketileset->GetTileWidth());i++){
            if(i == 0){
                spiketileset->RenderTile(0 + offset,associated.box.x + currentwidth - Camera::pos.x,associated.box.y - Camera::pos.y);
            }else if((i + 1) == (associated.box.w/spiketileset->GetTileWidth())){
                spiketileset->RenderTile(4 + offset,associated.box.x + currentwidth - Camera::pos.x,associated.box.y - Camera::pos.y);
            }else{
                spiketileset->RenderTile(2 + offset,associated.box.x + currentwidth - Camera::pos.x,associated.box.y - Camera::pos.y);
            }
            currentwidth += spiketileset->GetTileWidth();
        }
    }
    else if((associated.box.w == spiketileset->GetTileHeight()) && (associated.box.h > spiketileset->GetTileWidth())){
        int currentHeight = 0;
        int currentIndex = 1;
        for(int i = 0;i < (associated.box.h/spiketileset->GetTileWidth());i++){
            if(i == 0){
                spiketileset2->RenderTile(0 + offset,associated.box.x - Camera::pos.x,associated.box.y + currentHeight - Camera::pos.y);
            }else if((i + 1) == (associated.box.h/spiketileset->GetTileWidth())){
                spiketileset2->RenderTile(4 + offset,associated.box.x - Camera::pos.x,associated.box.y + currentHeight - Camera::pos.y);
            }else{
                spiketileset2->RenderTile(2 + offset,associated.box.x - Camera::pos.x,associated.box.y + currentHeight - Camera::pos.y);
            }
            currentHeight += spiketileset->GetTileWidth();
        }
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