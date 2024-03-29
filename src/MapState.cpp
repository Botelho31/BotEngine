#include "../include/MapState.h"
#include "../include/Camera.h"
#include "../include/Event.h"
#include "../include/GameData.h"
#include "../include/Player.h"
#include "../include/CameraFollower.h"

MapState::MapState(){
    this->windoweffects = new WindowEffects();
    Camera::pos.x = 0;
    Camera::pos.y = 0;
    centeronplayer = false;

    sizeOfTile = {50,50};
    printSize = {10,10};

    playerIcon = new GameObject();
    Sprite *playersprite = new Sprite(*playerIcon,"assets/img/beltest2.png");
    playersprite->SetScaleX(0.2,0.2);
    playerIcon->AddComponent(playersprite);
    AddObject(playerIcon);

    GameObject *celphoneObj = new GameObject();
    Sprite *celphone =  new Sprite(*celphoneObj,"assets/img/belcellphone.png",27,0.04,0,false);
    celphoneObj->box = celphoneObj->box.Added(-7,-7);
    celphoneObj->AddComponent(celphone);
    CameraFollower *camfollow =  new CameraFollower(*celphoneObj);
    celphoneObj->AddComponent(camfollow);
    AddObject(celphoneObj);

    Vec2 size = GameData::GetSizeOfPng("assets/img/FAKEWALL.png");
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
    if(input->KeyPress(ESCAPE_KEY)){
        popRequested = true;
    }
    if(input->KeyPress(SDLK_m)){
        popRequested = true;
    }
    if(input->IsKeyDown(SDLK_EQUALS)){
        float dif = 10 * dt;
        Camera::pos = playerIcon->box.GetCenter().Added(-Camera::window.x/2,-Camera::window.y/2);
        printSize = printSize.Added(dif,dif);
        Component *playercomp = playerIcon->GetComponent("Sprite");
        Sprite *playersprite = dynamic_cast<Sprite*>(playercomp);
        playersprite->SetScaleX((2*printSize.x)/100,(4*printSize.y)/200);
    }
    if(input->IsKeyDown(SDLK_MINUS)){
        float dif = 10 * dt;
        Camera::pos = playerIcon->box.GetCenter().Added(-Camera::window.x/2,-Camera::window.y/2);
        printSize = printSize.Added(-dif,-dif);
        Component *playercomp = playerIcon->GetComponent("Sprite");
        Sprite *playersprite = dynamic_cast<Sprite*>(playercomp);
        playersprite->SetScaleX((2*printSize.x)/100,(4*printSize.y)/200);
    }


}

void MapState::Render(){
    windoweffects->DrawToScreen(0,0,0,255);
    int x = 0;
    Rect lastmapbox;
    for(int i = 0;i < maps.size();i++){
        maps[i]->printed = false;
    }
    // if(maps.size() > 0){
    //     PrintMap(maps[0],{0,0});
    // }
    for(int i = 0;i < maps.size();i++){
        PrintMap(maps[0],{0,0});
    }
    RenderArray();
    windoweffects->Render();
}

void MapState::PrintMap(MapState::Map *map,Vec2 pos){
    if(map->printed){
        return;
    }else{
        if(map->mapInfoFile == GameData::checkpointMapInfo){
            Vec2 playerpos = Player::player->GetPosition();
            playerpos.x /= sizeOfTile.x/printSize.x;
            playerpos.y /= sizeOfTile.y/printSize.y;
            playerIcon->box.SetCenter(playerpos.Added(pos.x,pos.y));
            if(!centeronplayer){    
                Camera::pos = playerIcon->box.GetCenter().Added(-Camera::window.x/2,-Camera::window.y/2);
                centeronplayer = true;
            }

        }
        PrintTileMap(map,pos);
        for(int i = 0;i < map->portals.size();i++){
            Vec2 frommap = ApproximateToSideOfMap(map,map->portals[i]->PortalBox);
            frommap.x /= sizeOfTile.x/printSize.x;
            frommap.y /= sizeOfTile.y/printSize.y;
            Map *mappointer = nullptr;
            for(int j = 0;j < maps.size();j++){
                if(maps[j]->mapInfoFile == map->portals[i]->mapInfoTo){
                    mappointer = maps[j];
                }
            }
            if(mappointer){
                Vec2 tomap = ApproximateToSideOfMap(mappointer,map->portals[i]->PortalPosTo);
                tomap.x /= sizeOfTile.x/printSize.x;
                tomap.y /= sizeOfTile.y/printSize.y;
                PrintMap(mappointer,frommap.Added(pos.x,pos.y).Added(-tomap.x,-tomap.y));
            }
            
        }
    }
}

Vec2 MapState::ApproximateToSideOfMap(Map *map,Rect pos){
    pos.Transform(pos.GetCenter().x,pos.GetCenter().y);
    if(pos.x <= 30){
        pos.x = 0;
    }
    if(pos.y <= 30){
        pos.y = 0;
    }

    if((pos.x + pos.w) >= ((map->width*sizeOfTile.x) - 30)){
        pos.x = (map->width*sizeOfTile.x);
    }

    if((pos.y + pos.h) >= ((map->height*sizeOfTile.y) - 30)){
        pos.y = (map->height*sizeOfTile.y);
    }

    return pos.GetOrigin();
}

Vec2 MapState::ApproximateToSideOfMap(Map *map,Vec2 pos){
    if(pos.x <= 30){
        pos.x = 0;
    }
    if(pos.y <= 30){
        pos.y = 0;
    }

    if(pos.x >= ((map->width*sizeOfTile.x) - 30)){
        pos.x = (map->width*sizeOfTile.x);
    }

    if(pos.y >= ((map->height*sizeOfTile.y) - 30)){
        pos.y = (map->height*sizeOfTile.y);
    }

    return pos;
}

void MapState::GetMapsInfo(){
    std::string mapfile;
    std::fstream FileReaderpermap;
    std::string checkline;
    std::cout << GameData::listOfDiscoveredMaps.size() << std::endl;
    for(int i = 0;i < GameData::listOfDiscoveredMaps.size();i++){
        mapfile = GameData::listOfDiscoveredMaps[i];
        FileReaderpermap.open(mapfile);
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
                else if(checkline == "Portal"){        
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
                else if(checkline == "FakeWall"){
                    Vec2 pos;
                    std::string sprite;
                    Vec2 sizeofsprite;
                    while(checkline != "pos"){
                        FileReaderpermap >> checkline;
                    } 
                    FileReaderpermap >> pos.x;
                    FileReaderpermap >> pos.y;
                    FileReaderpermap >> checkline;
                    FileReaderpermap >> sprite;
                    FileReaderpermap >> checkline;
                    FileReaderpermap >> checkline;
                    sizeofsprite = GameData::GetSizeOfPng(sprite);
                    Rect fakewall = Rect(pos.x,pos.y,sizeofsprite.x,sizeofsprite.y);
                    map->fakewalls.push_back(fakewall);
                }
            }
            maps.emplace_back(map);
            FileReaderpermap.close();
        }else{
            ENDLINE
            std::cout << "No Map File Found: " << mapfile << std::endl; //Printa um erro caso nao consiga dar load na file
            FileReaderpermap.close();
        }

    }
    FileReaderpermap.close();

}

void MapState::GetMapSize(MapState::Map *map){
    map->tileMatrix = GameData::ParseTileMap(map->mapFile,map->width,map->height,map->depth);
    for(int i = map->tileMatrix.size();i >= 0;i --){
        if(i < (((map->depth - map->collisionDepthOffset - 1) * (map->width*map->height)))){
            map->tileMatrix.erase(map->tileMatrix.begin() + i);
        }
    }
}

void MapState::PrintTileMap(MapState::Map *map,Vec2 pos){
    Vec2 aproxprintsize = printSize;
    Rect maprect = map->GetMapRect(aproxprintsize).Added(pos.x -Camera::pos.x,pos.y -Camera::pos.y);
    maprect.x += 2;
    maprect.y += 2;
    maprect.w -= 4;
    maprect.h -= 4;
    // maprect.w = round(maprect.w);
    // maprect.h = round(maprect.h);
    windoweffects->FillRect(maprect,map->r,map->g,map->b,255);
    map->printed = true;

    for(int i = 0;i < map->height;i ++){
        for(int j = 0;j < map->width;j++){
            if(map->At(j,i) >= 0){
                Rect printtileRect =  Rect(j*printSize.x,i*printSize.y,ceil(printSize.x) + 1,ceil(printSize.y) + 1).Added(pos.x -Camera::pos.x,pos.y -Camera::pos.y);            
                windoweffects->FillRect(printtileRect,0,0,0,255);
            }
        }
    }
    for(int i = 0;i < map->fakewalls.size();i++){
        Rect fakewall = map->fakewalls[i];
        fakewall.x /= sizeOfTile.x/printSize.x; 
        fakewall.y /= sizeOfTile.y/printSize.y;
        fakewall.w /= sizeOfTile.x/printSize.x;
        fakewall.h /= sizeOfTile.y/printSize.y;
        windoweffects->FillRect(fakewall.Added(pos.x -Camera::pos.x,pos.y -Camera::pos.y),0,0,0,255);
    }
}

void MapState::Start(){
    Camera::UnFollow();
    StartArray();
    GetMapsInfo();
    for(int i = 0;i < maps.size();i++){
        GetMapSize(maps[i]);
    //     std::cout << maps[i]->mapInfoFile << std::endl;
    //     std::cout << "\tWidth: " << maps[i]->width << std::endl;
    //     std::cout << "\tHeight: " << maps[i]->height << std::endl;
    //     std::cout << "\tCollision Depth: " << maps[i]->collisionDepthOffset << std::endl;
    //     for(int j = 0;j < maps[i]->portals.size() ;j++){
    //         std::cout << "\t" << maps[i]->portals[j]->mapInfoTo << std::endl;
    //         std::cout << "\t\t" << maps[i]->portals[j]->PortalBox.x << " " << maps[i]->portals[j]->PortalBox.y << std::endl;
    //         std::cout << "\t\t" << maps[i]->portals[j]->PortalPosTo.x << " " << maps[i]->portals[j]->PortalPosTo.y << std::endl;
    //     }
    }
    Game::GetInstance().GetMusic()->Open("assets/audio/musics/belpause.ogg");
    Game::GetInstance().GetMusic()->Play();
}

void MapState::Resume(){

}

void MapState::Pause(){

}