#include "../include/TileMapInfo.h"
#include "../include/Minion.h"
#include "../include/Event.h"

TileMapInfo::TileMapInfo(std::string file){
    Open(file);
}

void TileMapInfo::Open(std::string file){
    std::fstream FileReader;
    FileReader.open(file.c_str());
    std::string checkline;
    if (FileReader.is_open()) {
        while (!FileReader.eof()) {
            FileReader >> checkline;    //Checa as palavras do grafo
            if(checkline == "Portal"){        
                Rect portalbox;
                Vec2 portalloc;
                std::string tilemapfile,tilemapinfofile;
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
                FileReader >> tilemapfile;
                FileReader >> checkline;
                FileReader >> tilemapinfofile;

                GameObject *eventObj = new GameObject();
                Event *event = new Event(*eventObj,portalbox,Event::PORTAL,tilemapfile,tilemapinfofile,portalloc);
                eventObj->AddComponent(event);
                Game::GetInstance().GetCurrentState().AddObject(eventObj);

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
                int place = Game::GetInstance().GetCurrentState().GetObjectPlaceAtLine("TileMap");
                if(place >= 0){
                    std::weak_ptr<GameObject> tileweakptr =  Game::GetInstance().GetCurrentState().AddObject(tileObj,place);
                }else{
                    std::weak_ptr<GameObject> tileweakptr =  Game::GetInstance().GetCurrentState().AddObject(tileObj);
                }
            }
        }
    }else{
        std::cout << "No TileMapInfo File for: " << file << std::endl; //Printa um erro caso nao consiga dar load na file
    }
    FileReader.close();
}