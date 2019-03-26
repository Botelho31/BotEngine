#include "../include/TileSet.h"
#include "../include/GameObject.h"

TileSet::TileSet(int tileWidth,int tileHeight,Resources *resources,std::string file) : resources(resources){
    owner = new GameObject();
    tileset = new Sprite(*owner,resources,file);
    this->tileWidth = tileWidth;
    this->tileHeight = tileHeight;
    if(tileset->IsOpen()){
        rows = tileset->GetHeight() / tileHeight;
        columns = tileset->GetWidth() / tileWidth;
    }else{
        std::cout << "Failed to open tileset: " << file << std::endl; 
    }
}

TileSet::~TileSet(){
    if(owner != nullptr){
        delete owner;
    }
    if(tileset != nullptr){
        delete tileset;
    }
}

void TileSet::RenderTile(unsigned index,float x,float y){
    if(index < (rows*columns)){
        int row = (index/columns);
        int column = (index%columns);
        tileset->SetClip(column * tileWidth,row * tileHeight,tileWidth,tileHeight);
        tileset->Render(x,y);

    }else{
        // std::cout << "Tile Requested Out of Index: " << rows*columns  << std::endl;
        // std::cout << "Index: " << index << " X: " << x << " Y: " << y << std::endl;
    }
}

int TileSet::GetTileHeight(){
    return tileHeight;
}

int TileSet::GetTileWidth(){
    return tileWidth;
}