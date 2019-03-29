#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef TILESET_H
    #define TILESET_H
    #include "Sprite.h"
    #include "Resources.h"

    class TileSet{
        public:
            TileSet(int tileWidth,int tileHeight,std::string file);
            ~TileSet();
            void RenderTile(unsigned index,float x,float y);
            int GetTileWidth();
            int GetTileHeight();
        private:
            GameObject *owner;
            Sprite *tileset;
            int rows;
            int columns;
            int tileWidth;
            int tileHeight;
    };
#endif
