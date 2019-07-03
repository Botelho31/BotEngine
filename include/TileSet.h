#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef TILESET_H
    #define TILESET_H
    #include "Sprite.h"
    #include "Resources.h"

    class TileSet{
        public:
            TileSet(GameObject* owner,int tileWidth,int tileHeight,std::vector<std::string> files);
            ~TileSet();
            void RenderTile(int index,float x,float y);
            int GetTileWidth();
            int GetTileHeight();

            int GetNumberOfTiles();
            void Flip(bool horizontal = true);
        private:
            GameObject *owner;
            std::vector<Sprite*> tilesets;
            std::vector<int> rows;
            std::vector<int> columns;
            int tileWidth;
            int tileHeight;
    };
#endif
