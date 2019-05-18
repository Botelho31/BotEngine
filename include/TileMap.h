#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef TILEMAP_H
    #define TILEMAP_H
    #include "GameObject.h"
    #include "Component.h"
    #include "TileSet.h"
    #include "TileMapInfo.h"

    class TileMap : public Component{
        public:
            TileMap(GameObject& associated,std::string file,TileSet* tileSet);
            ~TileMap();
            void Load(std::string file);
            void LoadInfo(std::string file);    //Load the TileMapInfo
            void LoadTileColliders();   //Load the tileColliders
            void SetTileSet(TileSet *tileSet);
            int& At(int x,int y,int z = 0);
            void RenderLayer(int layer,int cameraX = 0,int cameraY = 0);

            int AtLocation(int x,int y); //TO GET THE TILE AT MAP COORDINATES
            void InsertAtLocation(int x,int y,int numberoftile); //Changes the tile at a location(Not used currently)
            
            void Start();
            void Render();
            void Update(float dt);
            bool Is(std::string type);

            int GetWidth();
            int GetHeight();
            int GetDepth();

            static std::vector<std::weak_ptr<Component>> tiles;
            private:
            std::vector<int> tileMatrix;
            TileSet* tileSet;
            TileMapInfo* tileMapInfo;
            int mapWidth;
            int mapHeight;
            int mapDepth;
    };

#endif