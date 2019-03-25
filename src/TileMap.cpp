#include "../include/TileMap.h"

TileMap::TileMap(GameObject& associated,std::string file,TileSet* tileSet) : Component(associated){
    this->tileSet = tileSet;
    Load(file);
}

void TileMap::Load(std::string file){
    std::ifstream FileReader;
    FileReader.open(file);
    std::stringstream num;
    std::string number;
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
    }
    FileReader.close();
}

void TileMap::SetTileSet(TileSet* tileSet){
    this->tileSet = tileSet;
}

int& TileMap::At(int x,int y,int z){
    return x;
}


void TileMap::Render(){

}

void TileMap::Update(float dt){

}

bool TileMap::Is(std::string type){

}

void TileMap::RenderLayer(int layer,int cameraX,int cameraY){

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