#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef STATBAR_H
    #define STATBAR_H
    #include "Component.h"
    #include "Physics.h"
    #include "Sprite.h"
    #include "TileSet.h"

    class StatBar : public Component{
        public:
            StatBar(GameObject& associated,std::string emptybarfile,std::string fullbarfile,Vec2 tilesize,int max);
            ~StatBar();
            void Update(float dt);
            void Render();
            bool Is(std::string type);

            void SetCurrent(int current,bool sum = true);

            int GetCurrent();
            int GetMax();
        private:
            Vec2 tilesize;
            int perblockratio;
            int max;
            int current;
            GameObject *emptybar;
            TileSet *fullbartileset;
    };

#endif