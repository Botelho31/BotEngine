#include "../include/TileMapInfo.h"
#include "../include/Minion.h"

TileMapInfo::TileMapInfo(std::string file){
    Open(file);
}

void TileMapInfo::Open(std::string file){
    if((!portals.empty()) || (!portalfiles.empty())){
        portals.clear();
        portalfiles.clear();
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
                minionObj->box.SetCenter(MinionPos);
                minionObj->AddComponent(minion);
                Game::GetInstance().GetCurrentState().AddObject(minionObj);
            }
            if(checkline == "MovingTile"){
                Vec2 start;
                Vec2 dest;
                float speed;
                bool circular;
                while(checkline != "startX"){
                    FileReader >> checkline;
                } 
                FileReader >> start.x;
                FileReader >> checkline;
                FileReader >> start.y;
                FileReader >> checkline;
                FileReader >> speed;
                FileReader >> checkline;
                FileReader >> dest.x;
                FileReader >> checkline;
                FileReader >> dest.y;
                FileReader >> checkline;
                FileReader >> circular;
                GameObject *tileObj = new GameObject();
                MovingTile *movingtile = new MovingTile(*tileObj,speed,start,dest,circular);
                tileObj->AddComponent(movingtile);
                std::weak_ptr<GameObject> tileweakptr =  Game::GetInstance().GetCurrentState().AddObject(tileObj);
            }
        }
    }else{
        std::cout << "No TileMapInfo File for: " << file << std::endl; //Printa um erro caso nao consiga dar load na file
    }
    FileReader.close();
}