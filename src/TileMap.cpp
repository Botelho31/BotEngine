#include "../include/TileMap.h"

TileMap::TileMap(GameObject& associated,std::string file,TileSet* tileSet) : Component(associated){
    this->tileSet = tileSet;
    Load(file);
}

TileMap::~TileMap(){
    delete tileSet;
    if(tilemapinfo){
        delete tilemapinfo;
    }
}

void TileMap::Load(std::string file){
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
}

void TileMap::LoadInfo(std::string file){
    if(this->tilemapinfo){
        this->tilemapinfo->Open(file);
    }else{
        this->tilemapinfo = new TileMapInfo(file);
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
        // std::cout << "Out of Bounds" << std::endl;
        return -1;
    }
}

// Vec2 TileMap::FindPortalLoc(int tile){
//     int count = 0;
//     std::vector<Vec2> portallocs;
//     for(int i = 0;i < tileMatrix.size();i++){
//         if((tileMatrix[i] == tile) && (count == 0)){
//             Vec2 tileloc = Vec2(((i % this->mapWidth)-1) * tileSet->GetTileWidth(),(i/this->mapWidth) * tileSet->GetTileHeight());
//             portallocs.push_back(tileloc);
//             count ++;
//         }
//         if((tileMatrix[i] == tile) && (count == 1)){
//             Vec2 tileloc = Vec2(((i % this->mapWidth)-1) * tileSet->GetTileWidth(),(i/this->mapWidth) * tileSet->GetTileHeight());
//             portallocs.push_back(tileloc);
//         }
//     }
//     Vec2 portalloc;
//     portalloc.x = portallocs[0].x + (portallocs[1].x - portallocs[0].x)/2;
//     portalloc.y = portallocs[0].y + (portallocs[1].y - portallocs[0].y)/2;
//     return portalloc;
// }

void TileMap::RenderLayer(int layer,int cameraX,int cameraY){
    for(int h = 0;h < this->mapHeight;h++){
        for(int w = 0;w < this->mapWidth;w++){
            tileSet->RenderTile(At(w,h,layer),(tileSet->GetTileWidth() * w) - cameraX,(tileSet->GetTileHeight() * h) - cameraY );
        }
    }
}

void TileMap::Render(){
    for(int d = 0;d < this->mapDepth;d++){
        RenderLayer(d,associated.box.x,associated.box.y);
    }
}

void TileMap::Update(float dt){

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