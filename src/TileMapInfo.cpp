#include "../include/TileMapInfo.h"
#include "../include/Minion.h"

TileMapInfo::TileMapInfo(std::string file){
    portalfiles.clear();
    portals.clear();
    Open(file);
}

void TileMapInfo::Open(std::string file){
    if((!portals.empty()) || (!portalfiles.empty())){
        portals.clear();
        portalfiles.empty();
    }
    std::fstream FileReader;
    FileReader.open(file.c_str());
    std::string checkline;
    if (FileReader.is_open()) {
        while (!FileReader.eof()) {
            FileReader >> checkline;    //Checa as palavras do grafo
            if(checkline == "Portal"){        
                int ID = 0;
                Vec2 portalloc;
                std::string tilemapfile,tilemapinfofile;
                std::vector<std::string> tilemapfiles;
                while(checkline != "portalID"){
                    FileReader >> checkline;
                }   
                FileReader >> ID;
                FileReader >> checkline;
                FileReader >> portalloc.x;
                FileReader >> checkline;
                FileReader >> portalloc.y;
                portals.insert({ID,portalloc});   
                FileReader >> checkline;
                FileReader >> tilemapfile;
                FileReader >> checkline;
                FileReader >> tilemapinfofile;
                tilemapfiles.push_back(tilemapfile);
                tilemapfiles.push_back(tilemapinfofile);
                portalfiles.insert({ID,tilemapfiles});

            }
            if(checkline == "Minion"){       
                Vec2 MinionPos;
                while(checkline != "minionX"){
                    FileReader >> checkline;
                } 
                FileReader >> MinionPos.x;
                FileReader >> checkline;
                FileReader >> MinionPos.y;
                GameObject *minionObj = new GameObject();
                Minion *minion = new Minion(*minionObj);
                minionObj->box.x = MinionPos.x;
                minionObj->box.y = MinionPos.y;
                minionObj->AddComponent(minion);
                Game::GetInstance().GetCurrentState().AddObject(minionObj);
            }
            if(checkline == "MovingTile"){
                Vec2 tilePos;
                while(checkline != "tileX"){
                    FileReader >> checkline;
                } 
                FileReader >> tilePos.x;
                FileReader >> checkline;
                FileReader >> tilePos.y;
                GameObject *tileObj = new GameObject();
                MovingTile *movingtile = new MovingTile(*tileObj);
                tileObj->box.x = tilePos.x;
                tileObj->box.y = tilePos.y;
                tileObj->AddComponent(movingtile);
                std::weak_ptr<GameObject> tileweakptr =  Game::GetInstance().GetCurrentState().AddObject(tileObj);
                movingtiles.emplace_back(tileweakptr);
            }
        }
    }else{
        std::cout << "No TileMapInfo File for: " << file << std::endl; //Printa um erro caso nao consiga dar load na file
    }
    FileReader.close();
}