#include "../include/MapState.h"
#include "../include/Camera.h"
#include "../include/Event.h"
#include "../include/GameData.h"
#include "../include/Player.h"

MapState::MapState(){
    this->windoweffects = new WindowEffects();
}

MapState::~MapState(){
    Camera::Follow(Player::player->GetAssociated());
    delete windoweffects;
}

void MapState::LoadAssets(){

}

void MapState::Update(float dt){
    Camera::Update(dt);
    InputManager *input = &(InputManager::GetInstance());

	if(input->QuitRequested()){
		quitRequested = true;
	}
    if(input->KeyPress(ESCAPE_KEY)){
        popRequested = true;
    }


}

void MapState::Render(){
    windoweffects->DrawToScreen(0,0,0,255);
    int x = 0;
    for(int i = 0;i < maps.size();i++){
        Rect mapbox = maps[i]->GetMapRect();
        mapbox.x += x;
        windoweffects->FillRect(mapbox,255,255,255,255);
        x += mapbox.w;
    }
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
                        portal->PortalPos = portalbox.GetCenter();
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

void MapState::GetMapSize(Map *map){
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
    GetMapsInfo("assets/map/info/listOfMaps.txt");
    for(int i = 0;i < maps.size();i++){
        GetMapSize(maps[i]);
        std::cout << maps[i]->mapInfoFile << std::endl;
        std::cout << "\tWidth: " << maps[i]->width << std::endl;
        std::cout << "\tHeight: " << maps[i]->height << std::endl;
        std::cout << "\tCollision Depth: " << maps[i]->collisionDepthOffset << std::endl;
        for(int j = 0;j < maps[i]->portals.size() ;j++){
            std::cout << "\t" << maps[i]->portals[j]->mapInfoTo << std::endl;
            std::cout << "\t\t" << maps[i]->portals[j]->PortalPos.x << " " << maps[i]->portals[j]->PortalPos.y << std::endl;
            std::cout << "\t\t" << maps[i]->portals[j]->PortalPosTo.x << " " << maps[i]->portals[j]->PortalPosTo.y << std::endl;
        }
    }
}

void MapState::Resume(){

}

void MapState::Pause(){

}