#include "../include/TileMapInfo.h"

TileMapInfo::TileMapInfo(std::string file){
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
            if(checkline == "Portal"){        //se for igual a node entra e espera checar em ID
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
        }
    }else{
        std::cout << "Error reading TileMapInfo File: " << file << std::endl; //Printa um erro caso nao consiga dar load na file
    }
    FileReader.close();
}