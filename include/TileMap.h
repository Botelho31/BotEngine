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
            TileMap(GameObject& associated,std::string file,TileSet* tileSet);
            void Load(std::string file);
            void SetTileSet(TileSet *tileSet);
            int& At(int x,int y,int z = 0);
            void RenderLayer(int layer,int cameraX = 0,int cameraY = 0);
            
            void Render();
            void Update(float dt);
            bool Is(std::string type);

            int GetWidth();
            int GetHeight();
            int GetDepth();
        private:
            std::vector<int> tileMatrix;
            TileSet* tileSet;
            int mapWidth;
            int mapHeight;
            int mapDepth;
    };

#endif