#include "../include/StatBar.h"
#include "../include/Collider.h"
#include "../include/Camera.h"

StatBar::StatBar(GameObject& associated,std::string emptybarfile,std::string fullbarfile,Vec2 tilesize,int perblockratio,int max) : Component(associated){
    this->perblockratio = perblockratio;
    this->max = max;
    this->tilesize = tilesize;
    current = max;

    GameObject *ownerObj = new GameObject();
    std::vector<std::string> tilesprites;
    tilesprites.push_back(fullbarfile);
    fullbartileset = new TileSet(ownerObj,tilesize.x,tilesize.y,tilesprites);

    GameObject *owner2Obj = new GameObject();
    std::vector<std::string> tilesprites2;
    tilesprites2.push_back(emptybarfile);
    emptybartileset = new TileSet(owner2Obj,tilesize.x,tilesize.y,tilesprites);
}

StatBar::~StatBar(){
    delete fullbartileset;
    delete  emptybartileset;
}

void StatBar::Update(float dt){

}

void StatBar::Render(){
    int currentwidth = 0;
    for(int i = 0;i < (max/perblockratio);i++){
        if(i == 0){
            emptybartileset->RenderTile(0,associated.box.x + currentwidth - Camera::pos.x,associated.box.y - Camera::pos.y);
        }else if((i + 1) == (max/perblockratio)){
            emptybartileset->RenderTile(emptybartileset->GetNumberOfTiles() - 1,associated.box.x + currentwidth - Camera::pos.x,associated.box.y - Camera::pos.y);
        }else{
            emptybartileset->RenderTile(3,associated.box.x + currentwidth - Camera::pos.x,associated.box.y - Camera::pos.y);
        }
        currentwidth += emptybartileset->GetTileWidth();
    }
    currentwidth = 0;
    for(int i = 0;i < (current/perblockratio);i++){
        if(i == 0){
            fullbartileset->RenderTile(0,associated.box.x + currentwidth - Camera::pos.x,associated.box.y - Camera::pos.y);
        }else if((i + 1) == max/perblockratio){
            fullbartileset->RenderTile(fullbartileset->GetNumberOfTiles() - 1,associated.box.x + currentwidth - Camera::pos.x,associated.box.y - Camera::pos.y);
        }else{
            fullbartileset->RenderTile(1,associated.box.x + currentwidth - Camera::pos.x,associated.box.y - Camera::pos.y);
        }
        currentwidth += fullbartileset->GetTileWidth();
    }

}

bool StatBar::Is(std::string type){
    if(type == "StatBar"){
        return true;
    }else{
        return false;
    }
}

void StatBar::SetCurrent(int current,bool sum){
    if(sum){
        this->current += current;
    }else{
        this->current = current;
    }
}  

int StatBar::GetCurrent(){
    return current;
}

int StatBar::GetMax(){
    return max;
}