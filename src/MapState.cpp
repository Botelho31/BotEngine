#include "../include/MapState.h"
#include "../include/Camera.h"
#include "../include/Event.h"
#include "../include/GameData.h"
#include "../include/Player.h"

MapState::MapState(){
    this->windoweffects = new WindowEffects();
    Camera::pos.x = 0;
    Camera::pos.y = 0;

    playerIcon = new GameObject();
    Sprite *playersprite = new Sprite(*playerIcon,"assets/img/beltest2.png");
    playersprite->SetScaleX(0.3,0.3);
    playerIcon->AddComponent(playersprite);
    AddObject(playerIcon);
}

MapState::~MapState(){
    Camera::Follow(Player::player->GetAssociated());
    delete windoweffects;
    std::cout << "Cleared "<< objectArray.size() << " Objects" << std::endl;
    objectArray.clear();
}

void MapState::LoadAssets(){

}

void MapState::Update(float dt){
    UpdateArray(dt);
    Camera::Update(dt);
    InputManager *input = &(InputManager::GetInstance());

	if(input->QuitRequested()){
		quitRequested = true;
	}
    if(input->KeyPress(SDLK_m)){
        popRequested = true;
    }


}

void MapState::Render(){
    windoweffects->DrawToScreen(0,0,0,255);
    int x = 0;
    Rect lastmapbox;
    for(int i = 0;i < maps.size();i++){
        maps[i]->printed = false;
    }
    if(maps.size() > 0){
        PrintMap(maps[0],{0,0});
    }
    RenderArray();
}

void MapState::PrintMap(MapState::Map *map,Vec2 pos){
    if(map->printed){
        return;
    }else{
        if(map->mapInfoFile == GameData::checkpointMapInfo){
            Vec2 playerpos = Player::player->GetPosition();
            playerpos.x /= 5;
            playerpos.y /= 5;
            playerIcon->box.SetCenter(playerpos.Added(pos.x,pos.y));
        }
        windoweffects->FillRect(map->GetMapRect().Added(pos.x -Camera::pos.x,pos.y -Camera::pos.y),map->r,map->g,map->b,255);
        map->printed = true;
        for(int i = 0;i < map->portals.size();i++){
            Vec2 frommap = ApproximateToSideOfMap(map,map->portals[i]->PortalBox);
            frommap.x /= 5;
            frommap.y /= 5;
            Map *mappointer = nullptr;
            for(int j = 0;j < maps.size();j++){
                if(maps[j]->mapInfoFile == map->portals[i]->mapInfoTo){
                    mappointer = maps[j];
                }
            }
            if(mappointer){
                Vec2 tomap = ApproximateToSideOfMap(mappointer,map->portals[i]->PortalPosTo);
                tomap.x /= 5;
                tomap.y /= 5;
                PrintMap(mappointer,frommap.Added(pos.x,pos.y).Added(-tomap.x,-tomap.y));
            }
            
        }
    }
}

Vec2 MapState::ApproximateToSideOfMap(Map *map,Rect pos){
    if(pos.x <= 20){
        pos.x = -20;
    }
    if(pos.y <= 20){
        pos.y = -20;
    }

    if((pos.x + pos.w) >= ((map->width*50) - 30)){
        pos.x = ((map->width*50) + 20);
    }

    if((pos.y + pos.h) >= ((map->height*50) - 30)){
        pos.y = ((map->height*50) + 20);
    }

    return pos.GetOrigin();
}

Vec2 MapState::ApproximateToSideOfMap(Map *map,Vec2 pos){
    if(pos.x <= 30){
        pos.x = -20;
    }
    if(pos.y <= 30){
        pos.y = -20;
    }

    if(pos.x >= ((map->width*50) - 30)){
        pos.x = ((map->width*50) + 20);
    }

    if(pos.y >= ((map->height*50) - 30)){
        pos.y = ((map->height*50) + 20);
    }

    return pos;
}

void MapState::GetMapsInfo(std::string maplistfile){
    std::fstream FileReader;
    FileReader.open(maplistfile);
    std::string mapfile;

    if (FileReader.is_open()) {
        while (!FileReader.eof()) {
            mapfile.clear();
            FileReader >> mapfile;
            std::stringstream mapPrefix;
            mapPrefix << "assets/map/info/" << mapfile;
            mapfile = mapPrefix.str();
            mapPrefix.clear();
            std::fstream FileReaderpermap;
            FileReaderpermap.open(mapfile);
            std::string checkline;
            if (FileReaderpermap.is_open()) {
                Map *map =  new Map();
                map->r  = (rand() % 255) + 1;
                map->g = (rand() % 255) + 1;
                map->b = (rand() % 255) + 1;
                map->mapInfoFile = mapfile;
                while (!FileReaderpermap.eof()){
                    FileReaderpermap >> checkline;
                    if(checkline == "MapInfo"){ 
                        std::string newmapfile; 
                        int collisionDepthOffset;      
                        while(checkline != "mapFile"){
                            FileReaderpermap >> checkline;
                        }   
                        FileReaderpermap >> newmapfile;
                        FileReaderpermap >> checkline;
                        FileReaderpermap >> collisionDepthOffset;
                        map->collisionDepthOffset = collisionDepthOffset;
                        map->mapFile = newmapfile;
                    }
                    if(checkline == "Portal"){        
                        Rect portalbox;
                        Vec2 portalloc;
                        std::string tilemapinfofile;
                        while(checkline != "portalBox"){
                            FileReaderpermap >> checkline;
                        }   
                        FileReaderpermap >> portalbox.x;
                        FileReaderpermap >> portalbox.y;
                        FileReaderpermap >> portalbox.w;
                        FileReaderpermap >> portalbox.h;
                        FileReaderpermap >> checkline;
                        FileReaderpermap >> portalloc.x;
                        FileReaderpermap >> checkline;
                        FileReaderpermap >> portalloc.y;  
                        FileReaderpermap >> checkline;
                        FileReaderpermap >> tilemapinfofile;
                        Portal *portal = new Portal();
                        portal->mapInfoTo = tilemapinfofile;
                        portal->PortalBox = portalbox;
                        portal->PortalPosTo = portalloc;
                        map->portals.emplace_back(portal);
                    }
                }
                maps.emplace_back(map);
            }else{
                ENDLINE
                std::cout << "No Map File Found: " << mapfile << std::endl; //Printa um erro caso nao consiga dar load na file
                FileReaderpermap.close();
            }
        }
    }else{
        ENDLINE
        std::cout << "No Maps File Found: " << maplistfile << std::endl; //Printa um erro caso nao consiga dar load na file
    }
    FileReader.close();
}

void MapState::GetMapSize(MapState::Map *map){
    std::string file = map->mapFile;
    if(GameData::GetExtension(file) == "tmx"){
        file = GameData::ParseTMX(file);
    }
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
                map->width = numint;
            }
            else if(iterator == 1){
                map->height = numint;
            }
            iterator ++;
        }
    }else{
        std::cout << "Couldnt get Map Sizes: " << file << std::endl;
    }
    FileReader.close();
}

void MapState::Start(){
    Camera::UnFollow();
    StartArray();
    GetMapsInfo("assets/map/info/listOfMaps.txt");
    for(int i = 0;i < maps.size();i++){
        GetMapSize(maps[i]);
        std::cout << maps[i]->mapInfoFile << std::endl;
        std::cout << "\tWidth: " << maps[i]->width << std::endl;
        std::cout << "\tHeight: " << maps[i]->height << std::endl;
        std::cout << "\tCollision Depth: " << maps[i]->collisionDepthOffset << std::endl;
        for(int j = 0;j < maps[i]->portals.size() ;j++){
            std::cout << "\t" << maps[i]->portals[j]->mapInfoTo << std::endl;
            std::cout << "\t\t" << maps[i]->portals[j]->PortalBox.x << " " << maps[i]->portals[j]->PortalBox.y << std::endl;
            std::cout << "\t\t" << maps[i]->portals[j]->PortalPosTo.x << " " << maps[i]->portals[j]->PortalPosTo.y << std::endl;
        }
    }
}

void MapState::Resume(){

}

void MapState::Pause(){

}