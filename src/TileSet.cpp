#include "../include/TileSet.h"
#include "../include/GameObject.h"

TileSet::TileSet(int tileWidth,int tileHeight,std::string file){
    GameObject* tmp;
    tileset = new Sprite(*tmp,file);
    this->tileWidth = tileWidth;
    this->tileHeight = tileHeight;
    if(tileset->IsOpen()){
        rows = tileset->GetWidth() / tileWidth;
        columns = tileset->GetHeight() / tileHeight;
    }else{
        std::cout << "Failed to open tileset: " << file << std::endl; 
    }
}

void TileSet::RenderTile(unsigned index,float x,float y){
    if(index < (rows*columns)){
        int row = (index/columns);
        int column = (index%columns);
        tileset->SetClip(row * tileHeight,column * tileWidth,tileWidth,tileHeight);
        tileset->Render(x,y);

    }else{
        std::cout << "Tile Requested Out of Index: " << rows*columns << std::endl;
    }
}

int TileSet::GetTileHeight(){
    return tileHeight;
}

int TileSet::GetTileWidth(){
    return tileWidth;
}