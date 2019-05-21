#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef EVENT_H
    #define EVENT_H
    #include "Rect.h"
    #include "Vec2.h"

    class Event : public Component{
        public:
            enum EventType{PORTAL,PLAYERDEATH,NOTHING};

            Event(GameObject& associated,EventType eventType,Rect box,std::string tileMap,std::string tileMapInfo,Vec2 portalloc,int collisionDepthOffset);
            Event(GameObject& associated,EventType eventType,float eventduration);
            ~Event();

            void Update(float dt);
            void Render();
            bool Is(std::string type);
            void NotifyCollision(GameObject& other);

            bool IsEventTimerOver();
            bool IsProcessing();
            void SetProcessing(bool processing);
            EventType GetType();
            Rect GetBox();

            int GetCollisionDepthOffset();
            Vec2 GetPortalLoc();
            std::string GetTileMap();
            std::string GetTileMapInfo();

        private:
            float eventduration;
            Timer* eventtimer;

            EventType eventType;
            bool processing;

            int collisionDepthOffset;
            Vec2 portalLoc;
            std::string tileMap;
            std::string tileMapInfo;
            
    };

#endif