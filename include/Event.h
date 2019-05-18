#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef EVENT_H
    #define EVENT_H
    #include "Rect.h"
    #include "Vec2.h"

    class Event : public Component{
        public:
            Event(GameObject& associated,Rect box,bool isPortal,std::string tileMap,std::string tileMapInfo,Vec2 portalloc);
            ~Event();

            void Update(float dt);
            void Render();
            bool Is(std::string type);
            void NotifyCollision(GameObject& other);

        private:
            bool isPortal;
            Vec2 portalLoc;
            std::string tileMap;
            std::string tileMapInfo;
            
    };

#endif