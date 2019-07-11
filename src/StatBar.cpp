#include "../include/StatBar.h"
#include "../include/Collider.h"
#include "../include/Camera.h"

StatBar::StatBar(GameObject& associated,std::string emptybarfile,std::string fullbarfile,Vec2 tilesize,int max) : Component(associated){
    this->max = max;
    this->tilesize = tilesize;
    current = max;

    GameObject *ownerObj = new GameObject();
    std::vector<std::string> tilesprites;
    tilesprites.push_back(fullbarfile);
    fullbartileset = new TileSet(ownerObj,tilesize.x,tilesize.y,tilesprites);

    this->emptybar = new GameObject();
    Sprite *sprite = new Sprite(*emptybar,emptybarfile);
    emptybar->AddComponent(sprite);
}

StatBar::~StatBar(){
    delete fullbartileset;
    delete emptybar;
}

void StatBar::Update(float dt){
    emptybar->Update(dt);
    emptybar->box.x = associated.box.x;
    emptybar->box.y = associated.box.y;
}

void StatBar::Render(){
    emptybar->Render();
    int currentWidth = 0;
    float percentage = float(current)/float(max);
    for(int i = 0;i < floor(float(fullbartileset->GetNumberOfTiles()) * percentage);i++){
        fullbartileset->RenderTile(i,associated.box.x + currentWidth - Camera::pos.x, associated.box.y - Camera::pos.y);
        currentWidth += fullbartileset->GetTileWidth();
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