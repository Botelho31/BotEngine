#include "../include/TileSet.h"
#include "../include/GameObject.h"

TileSet::TileSet(GameObject *owner,int tileWidth,int tileHeight,std::vector<std::string> files) : owner(owner){
    this->tileWidth = tileWidth;
    this->tileHeight = tileHeight;

    for(int i = 0;i < files.size();i++){
        tilesets.push_back(new Sprite(*owner,files[i]));
        owner->AddComponent(tilesets.back());

        if(tilesets[i]->IsOpen()){
            rows.push_back(tilesets[i]->GetHeight() / tileHeight);
            columns.push_back(tilesets[i]->GetWidth() / tileWidth);
        }else{
            std::cout << "Failed to open tileset: " << files[i] << std::endl; 
        }
    }
}

TileSet::~TileSet(){
    delete owner;
}

void TileSet::RenderTile(int index,float x,float y,bool debug){
    bool printing = true;
    int tilesetindex = 0;
    while(printing){
        if((index < (rows[tilesetindex]*columns[tilesetindex])) && (index >= 0)){
            int row = (index/columns[tilesetindex]);
            int column = (index%columns[tilesetindex]);
            tilesets[tilesetindex]->SetClip(column * tileWidth,row * tileHeight,tileWidth,tileHeight);
            tilesets[tilesetindex]->Render(x,y);
            printing = false;

        }else{
            if(index >= (rows[tilesetindex]*columns[tilesetindex])){
                if((tilesetindex + 1) < tilesets.size()){
                    index -= rows[tilesetindex]*columns[tilesetindex];
                    tilesetindex ++;
                }else{
                    printing = false;
                    std::cout << "Tile Requested Out of Index: " << rows[tilesetindex]*columns[tilesetindex]  << std::endl;
                    std::cout << "Index: " << index << " X: " << x << " Y: " << y << std::endl;
                }
            }else{
                printing = false;
            }
        }
    }
}

int TileSet::GetTileHeight(){
    return tileHeight;
}

int TileSet::GetTileWidth(){
    return tileWidth;
}

void TileSet::Flip(bool horizontal){
    if(horizontal){
        for(int i = 0;i < tilesets.size();i++){
            tilesets[i]->Flip();
        }
    }else{
        for(int i = 0;i < tilesets.size();i++){
            tilesets[i]->Flip(true);
        }
    }
}