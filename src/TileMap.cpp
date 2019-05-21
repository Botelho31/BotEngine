#include "../include/TileMap.h"
#include "../include/StageState.h"
#include "../include/Camera.h"
#include "../include/TileCollider.h"
#include "../include/GameData.h"

std::vector<std::weak_ptr<Component>> TileMap::tiles;

TileMap::TileMap(GameObject& associated,std::string file,TileSet* tileSet) : Component(associated){
    this->tileSet = tileSet;
    this->tileMapInfo = nullptr;
    Load(file);
}

TileMap::~TileMap(){
    delete tileSet;
    tiles.clear();
    if(tileMapInfo){
        delete tileMapInfo;
    }
}

void TileMap::Load(std::string file){
    if(GameData::GetExtension(file) == "tmx"){
        file = GameData::ParseTMX(file);
    }
    std::ifstream FileReader;
    FileReader.open(file);
    std::stringstream num;
    std::string number;
    tileMatrix.clear();
    int numint = 0;
    int iterator = 0;
    if (FileReader.is_open()) {
        while (!FileReader.eof()) {
            num.clear();
            std::getline(FileReader, number, ',');
            num << number;
            num >> numint;
            if(iterator == 0){
                this->mapWidth = numint;
            }
            else if(iterator == 1){
                this->mapHeight = numint;
            }
            else if(iterator == 2){
                this->mapDepth = numint;
            }
            else{
                this->tileMatrix.push_back(numint-1);
            }
            iterator ++;
        }
    }else{
        std::cout << "Couldnt open tilemap: " << file << std::endl;
    }
    FileReader.close();
    associated.box.w = this->mapWidth * tileSet->GetTileWidth();
    associated.box.h = this->mapHeight * tileSet->GetTileHeight();
    Camera::limit.x = associated.box.w;
    Camera::limit.y = associated.box.h;
    Camera::Correct();
    Camera::Update(0);
}

void TileMap::LoadInfo(std::string file){
    if(this->tileMapInfo){
        this->tileMapInfo->Open(file);
    }else{
        this->tileMapInfo = new TileMapInfo(file);
    }
}

void TileMap::Start(){
    LoadTileColliders();
}

void TileMap::LoadTileColliders(){
    tiles.clear();
    for(int h = 0;h < this->mapHeight;h++){
        for(int w = 0;w < this->mapWidth;w++){
            if((At(w,h,this->mapDepth - 1) + 1) > 0){
                GameObject* tileGO = new GameObject();
                TileCollider *tilecollider = new TileCollider(*tileGO,Rect(tileSet->GetTileWidth() * w,tileSet->GetTileHeight() * h,tileSet->GetTileWidth(),tileSet->GetTileHeight()));
                std::weak_ptr<Component> weakptrtile = tileGO->AddComponent(tilecollider);
                if(!weakptrtile.expired()){
                    tiles.push_back(weakptrtile);
                }
                Game::GetInstance().GetCurrentState().AddObject(tileGO);
            }
        }
    }
}

void TileMap::SetTileSet(TileSet* tileSet){
    this->tileSet = tileSet;
}

int& TileMap::At(int x,int y,int z){
    if((x < this->mapWidth) && (y < this->mapHeight) && (z < this->mapDepth)){
        int tilePlace = ( (y + (this->mapHeight * z)) * this->mapWidth) + x;
        return tileMatrix[tilePlace];
    }else{
        int error = -1;
        std::cout << "Tile not Located" << std::endl;
        return error;
    }
}

int TileMap::AtLocation(int x,int y){
    if(((x < (this->mapWidth * this->tileSet->GetTileWidth())) && (y < (this->mapHeight * this->tileSet->GetTileHeight()))) 
      && ( (x >= 0) && (y >= 0) ) ){
        
        int ytile,xtile;
        xtile = x / (this->tileSet->GetTileWidth());
        ytile = y / (this->tileSet->GetTileHeight());
        int tilePlace = ( (ytile + (this->mapHeight * (this->mapDepth - 1))) * this->mapWidth) + xtile;
        return tileMatrix[tilePlace];
    }else{
        return -1;
    }
}

void TileMap::InsertAtLocation(int x,int y,int numberoftile){
    if(((x < (this->mapWidth * this->tileSet->GetTileWidth())) && (y < (this->mapHeight * this->tileSet->GetTileHeight()))) 
      && ( (x >= 0) && (y >= 0) ) ){
        int ytile,xtile;
        xtile = x / (this->tileSet->GetTileWidth());
        ytile = y / (this->tileSet->GetTileHeight());
        int tilePlace = ( (ytile + (this->mapHeight * (this->mapDepth - 1))) * this->mapWidth) + xtile;
        tileMatrix[tilePlace] = numberoftile;
    }
}

void TileMap::RenderLayer(int layer,int cameraX,int cameraY){
    //PARALLAX
    int dif =  this->mapDepth - (layer + 1);
    float paxMult = 1/((float)dif*2);
    if(dif == 0){
        paxMult = 1;
    }

    for(int h = 0;h < this->mapHeight;h++){
        for(int w = 0;w < this->mapWidth;w++){
            tileSet->RenderTile(At(w,h,layer),(tileSet->GetTileWidth() * w) - cameraX * paxMult,(tileSet->GetTileHeight() * h) - cameraY);
        }
    }
}

void TileMap::Render(){
    for(int d = 0;d < this->mapDepth;d++){
        RenderLayer(d,associated.box.x,associated.box.y);
    }
}

void TileMap::Update(float dt){
    for(int i = (tiles.size() - 1); i >= 0;--i){
        if(tiles[i].expired()){
            tiles.erase(tiles.begin() + i);
        }
    }
}

bool TileMap::Is(std::string type){
    if(type == "TileMap"){
        return true;
    }else{
        return false;
    }
}

int TileMap::GetWidth(){
    return mapWidth;
}

int TileMap::GetHeight(){
    return mapHeight;
}

int TileMap::GetDepth(){
    return mapDepth;
}