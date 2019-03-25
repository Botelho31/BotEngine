#include "../include/TileMap.h"

TileMap::TileMap(GameObject& associated,std::string file,TileSet* tileSet) : Component(associated){
    this->tileSet = tileSet;
    Load(file);
}

TileMap::~TileMap(){
    delete tileSet;
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
    // std::cout << "X :" << x << " Y: " << y << " Z: " << z << std::endl;
    // std::cout << "Height: " << this->mapHeight << " Width: " << this->mapWidth << " Depth: " << this->mapDepth <<std::endl;
    if((x < this->mapWidth) && (y < this->mapHeight) && (z < this->mapDepth)){
        int tilePlace = ( (y + (this->mapHeight * z)) * this->mapWidth) + x;
        // std::cout << "TILEPLACE: "<< tilePlace << std::endl;
        // if(tileMatrix[tilePlace] < 9){
        //     std::cout << "0";
        // }
        // if(tileMatrix[tilePlace] == -1){
        //     std::cout << "0" << ",";
        // }else{
        //     std::cout << (tileMatrix[tilePlace] + 1) << ",";
        // }
        // if(((tilePlace + 1) % 25) == 0){
        //     std::cout << std::endl;
        // }

        return tileMatrix[tilePlace];
    }else{
        int error = -1;
        std::cout << "Tile not Located" << std::endl;
        return error;
    }
}

void TileMap::RenderLayer(int layer,int cameraX,int cameraY){
    for(int h = 0;h < this->mapHeight;h++){
        for(int w = 0;w < this->mapWidth;w++){
            tileSet->RenderTile(At(w,h,layer),tileSet->GetTileWidth() * w,tileSet->GetTileHeight() * h);
        }
    }
}

void TileMap::Render(){
    for(int d = 0;d < this->mapDepth;d++){
        RenderLayer(0,associated.box.x,associated.box.y);
        // associated.RequestDelete();
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