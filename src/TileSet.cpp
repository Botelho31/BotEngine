#include "../include/TileSet.h"
#include "../include/GameObject.h"

TileSet::TileSet(GameObject *owner,int tileWidth,int tileHeight,std::string file) : owner(owner){
    tileset = new Sprite(*owner,file);
    owner->AddComponent(tileset);
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
    delete owner;
}

void TileSet::RenderTile(unsigned index,float x,float y){
    if((index < (rows*columns)) && (index >= 0)){
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