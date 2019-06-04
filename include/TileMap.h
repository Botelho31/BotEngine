#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef TILEMAP_H
    #define TILEMAP_H
    #include "GameObject.h"
    #include "Component.h"
    #include "TileSet.h"

    class TileMap : public Component{
        public:
            TileMap(GameObject& associated,std::string file);
            ~TileMap();
            void Load(std::string file);

            std::string LoadInfo(std::string file);    //Load the Map Info
            void SpawnMobs(std::string file);   //Spawns the mobs
            void LoadTileColliders();   //Load the Tile Colliders

            void ExchangeMap(std::string infofile); // Exchanges maps completely
            
            void SetTileSet(TileSet *tileSet);
            int& At(int x,int y,int z = 0);
            void RenderLayer(int layer,int cameraX = 0,int cameraY = 0);

            int AtLocation(int x,int y); //TO GET THE TILE AT MAP COORDINATES
            void InsertAtLocation(int x,int y,int numberoftile); //Changes the tile at a location(Not used currently)
            
            void Start();
            void Render();
            void RenderForeGround();
            void Update(float dt);
            bool Is(std::string type);

            int GetWidth();
            int GetHeight();
            int GetDepth();

            static std::vector<std::weak_ptr<Component>> tiles;
            private:
            std::vector<int> tileMatrix;
            TileSet* tileSet;
            int mapWidth;
            int mapHeight;
            int mapDepth;
            int collisionDepthOffset;
            int parallaxDepthOffset;
    };

#endif