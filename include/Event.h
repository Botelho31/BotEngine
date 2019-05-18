#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef EVENT_H
    #define EVENT_H
    #include "Rect.h"
    #include "Vec2.h"

    class Event : public Component{
        public:
            enum EventType{PORTAL,NOTHING};

            Event(GameObject& associated,Rect box,EventType eventType,std::string tileMap,std::string tileMapInfo,Vec2 portalloc);
            ~Event();

            void Update(float dt);
            void Render();
            bool Is(std::string type);
            void NotifyCollision(GameObject& other);

            bool IsProcessing();
            void SetProcessing(bool processing);
            EventType GetType();
            Rect GetBox();

            Vec2 GetPortalLoc();
            std::string GetTileMap();
            std::string GetTileMapInfo();

        private:
            EventType eventType;
            bool processing;

            Vec2 portalLoc;
            std::string tileMap;
            std::string tileMapInfo;
            
    };

#endif