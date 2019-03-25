#include "../include/TileMap.h"

TileMap::TileMap(GameObject& associated,std::string file,TileSet* tileSet) : Component(associated){
    this->tileSet = tileSet;
    Load(file);
}

void TileMap::Load(std::string file){
     std::ifstream FileReader;
    FileReader.open(file);
    char output[100];
    if (FileReader.is_open()) {
        while (!FileReader.eof()) {
            FileReader >> output;
            std::cout << output  << std::endl;

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