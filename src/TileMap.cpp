#include "../include/TileMap.h"
#include "../include/StageState.h"
#include "../include/Camera.h"
#include "../include/TileCollider.h"
#include "../include/GameData.h"
#include "../include/Minion.h"
#include "../include/MovingTile.h"
#include "../include/FakeWall.h"
#include "../include/Boss.h"
#include "../include/Spike.h"


std::vector<std::weak_ptr<Component>> TileMap::tiles;

TileMap::TileMap(GameObject& associated,std::string file) : Component(associated){
    this->collisionDepthOffset = 0;
    this->parallaxDepthOffset = 0;
    Load(LoadInfo(file));
}

TileMap::~TileMap(){
    delete tileSet;
    tiles.clear();
}

void TileMap::Load(std::string file){
    tileMatrix = GameData::ParseTileMap(file,mapWidth,mapHeight,mapDepth);
    associated.box.w = this->mapWidth * tileSet->GetTileWidth();
    associated.box.h = this->mapHeight * tileSet->GetTileHeight();
    Camera::limit.x = associated.box.w;
    Camera::limit.y = associated.box.h;
    Camera::Correct();
    Camera::Update(0);
}

std::string TileMap::LoadInfo(std::string file){
    this->collisionDepthOffset = 0;
    this->parallaxDepthOffset = 0;
    std::string backgroundfile;
    bool backgroundParallax;
    Vec2 backgroundscale;
    std::vector<std::string> tilesetfiles;
    std::string mapfile;
    std::fstream FileReader;
    FileReader.open(file.c_str());
    std::string checkline;
    if (FileReader.is_open()) {
        GameData::AddMap(file);
        while (!FileReader.eof()) {
            FileReader >> checkline;    //Checa as palavras do grafo
            if(checkline == "MapInfo"){        
                while(checkline != "mapFile"){
                    FileReader >> checkline;
                }   
                FileReader >> mapfile;
                FileReader >> checkline;
                FileReader >> collisionDepthOffset;
                FileReader >> checkline;
                FileReader >> parallaxDepthOffset;
                FileReader >> checkline;
                FileReader >> checkline;
                while(checkline != "background"){
                    tilesetfiles.push_back(checkline);
                    FileReader >> checkline;
                }
                FileReader >> backgroundfile;
                FileReader >> backgroundParallax;
                FileReader >> backgroundscale.x;
                FileReader >> backgroundscale.y;
                StageState::ChangeBackground(backgroundfile,backgroundParallax,backgroundscale);
                GameObject *tilesetObj = new GameObject();
                TileSet *newtileset = new TileSet(tilesetObj,50,50,tilesetfiles);
	            SetTileSet(newtileset);
            }
        }
    }else{
        std::cout << "No TileMapInfo File for: " << file << std::endl; //Printa um erro caso nao consiga dar load na file
        return "";
    }
    FileReader.close();
    return mapfile;
}

void TileMap::SpawnMobs(std::string file){
    std::fstream FileReader;
    FileReader.open(file.c_str());
    std::string checkline;
    if (FileReader.is_open()) {
        while (!FileReader.eof()) {
            FileReader >> checkline;    //Checa as palavras do grafo
            if(checkline == "Portal"){        
                Rect portalbox;
                Vec2 portalloc;
                std::string tilemapinfofile;
                while(checkline != "portalBox"){
                    FileReader >> checkline;
                }   
                FileReader >> portalbox.x;
                FileReader >> portalbox.y;
                FileReader >> portalbox.w;
                FileReader >> portalbox.h;
                FileReader >> checkline;
                FileReader >> portalloc.x;
                FileReader >> checkline;
                FileReader >> portalloc.y;  
                FileReader >> checkline;
                FileReader >> tilemapinfofile;

                GameObject *eventObj = new GameObject();
                Event *event = new Event(*eventObj,Event::PORTAL,portalbox,tilemapinfofile,portalloc);
                eventObj->AddComponent(event);
                Game::GetInstance().GetCurrentState().AddObject(eventObj);

            }
            else if(checkline == "Minion"){       
                Vec2 MinionPos;
                while(checkline != "minionX"){
                    FileReader >> checkline;
                } 
                FileReader >> MinionPos.x;
                FileReader >> checkline;
                FileReader >> MinionPos.y;
                GameObject *minionObj = new GameObject();
                Minion *minion = new Minion(*minionObj);
                minionObj->box.SetCenter(MinionPos);
                minionObj->AddComponent(minion);
                Game::GetInstance().GetCurrentState().AddObject(minionObj);
            }
            else if(checkline == "MovingTile"){
                Vec2 start;
                Vec2 dest;
                float speed;
                bool circular;
                while(checkline != "startPos"){
                    FileReader >> checkline;
                } 
                FileReader >> start.x;
                FileReader >> start.y;
                FileReader >> checkline;
                FileReader >> speed;
                FileReader >> checkline;
                FileReader >> dest.x;
                FileReader >> dest.y;
                FileReader >> checkline;
                FileReader >> circular;
                GameObject *tileObj = new GameObject();
                MovingTile *movingtile = new MovingTile(*tileObj,speed,start,dest,circular);
                tileObj->AddComponent(movingtile);
                int place = Game::GetInstance().GetCurrentState().GetObjectPlaceAtLine("Player");
                Game::GetInstance().GetCurrentState().AddObject(tileObj,place);
            }
            else if(checkline == "FakeWall"){
                Vec2 pos;
                std::string sprite;
                bool breakable;
                while(checkline != "pos"){
                    FileReader >> checkline;
                } 
                FileReader >> pos.x;
                FileReader >> pos.y;
                FileReader >> checkline;
                FileReader >> sprite;
                FileReader >> checkline;
                FileReader >> breakable;
                GameObject *fakewallObj = new GameObject();
                FakeWall *fakewall = new FakeWall(*fakewallObj,sprite,breakable);
                fakewallObj->AddComponent(fakewall);
                fakewallObj->box.Transform(pos.x,pos.y);
                if(breakable){
                    int place = Game::GetInstance().GetCurrentState().GetObjectPlaceAtLine("Player");
                    Game::GetInstance().GetCurrentState().AddObject(fakewallObj,place);
                }else{
                    Game::GetInstance().GetCurrentState().AddObject(fakewallObj);
                }
            }
            else if(checkline == "Spike"){
                Rect spikebox;
                bool flipped;
                while(checkline != "spikeBox"){
                    FileReader >> checkline;
                }
                FileReader >> spikebox.x;
                FileReader >> spikebox.y;
                FileReader >> spikebox.w;
                FileReader >> spikebox.h;
                FileReader >> checkline;
                FileReader >> flipped;

                GameObject *spikeObj = new GameObject();
                spikeObj->box = spikebox;
                Spike *spike = new Spike(*spikeObj,{400,400},flipped);
                spikeObj->AddComponent(spike);
                int place = Game::GetInstance().GetCurrentState().GetObjectPlaceAtLine("Player");
                Game::GetInstance().GetCurrentState().AddObject(spikeObj,place);
            }
            else if(checkline == "Boss"){
                Vec2 pos;
                while(checkline != "pos"){
                    FileReader >> checkline;
                }
                FileReader >> pos.x;
                FileReader >> pos.y;
                GameObject *bossObj = new GameObject();
                bossObj->box.Transform(pos.x,pos.y);
                Boss *boss = new Boss(*bossObj);
                bossObj->AddComponent(boss);
                int place = Game::GetInstance().GetCurrentState().GetObjectPlaceAtLine("Player");
                Game::GetInstance().GetCurrentState().AddObject(bossObj,place);
            }
        }
    }else{
        std::cout << "No TileMapInfo File for: " << file << std::endl; //Printa um erro caso nao consiga dar load na file
    }
    FileReader.close();
}

void TileMap::ExchangeMap(std::string infofile){
    Load(LoadInfo(infofile));
    LoadTileColliders();
    SpawnMobs(infofile);
}

void TileMap::Start(){
    LoadTileColliders();
    SpawnMobs(GameData::checkpointMapInfo);
}

void TileMap::LoadTileColliders(){
    tiles.clear();
    for(int h = 0;h < this->mapHeight;h++){
        for(int w = 0;w < this->mapWidth;w++){
            if((At(w,h,this->mapDepth - 1 - collisionDepthOffset) + 1) > 0){
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
        int tilePlace = ( (ytile + (this->mapHeight * (this->mapDepth - 1 - this->collisionDepthOffset))) * this->mapWidth) + xtile;
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
    int dif =  (this->mapDepth - this->parallaxDepthOffset) - (layer + 1);
    float paxMult = 1/((float)dif*2);
    if((dif <= 0) || ((layer + 1) == (this->mapDepth - this->collisionDepthOffset))){
        paxMult = 1;
    }

    for(int h = 0;h < this->mapHeight;h++){
        for(int w = 0;w < this->mapWidth;w++){
            tileSet->RenderTile(At(w,h,layer),(tileSet->GetTileWidth() * w) - cameraX * paxMult,(tileSet->GetTileHeight() * h) - cameraY);
        }
    }
}

void TileMap::Render(){
    for(int d = 0;d < (this->mapDepth - collisionDepthOffset);d++){
        RenderLayer(d,associated.box.x,associated.box.y);
    }
}

void TileMap::RenderForeGround(){
    for(int d = (this->mapDepth - collisionDepthOffset);d < this->mapDepth;d++){
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