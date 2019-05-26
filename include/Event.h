#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef EVENT_H
    #define EVENT_H
    #include "Rect.h"
    #include "Vec2.h"

    class Event : public Component{
        public:
            enum EventType{PORTAL,PLAYERDEATH,PAUSE,UNPAUSE,QUITTOMAINSCREEN,NOTHING};

            Event(GameObject& associated,EventType eventType,Rect box,std::string tileMapInfo,Vec2 portalloc);
            Event(GameObject& associated,EventType eventType,float eventduration = 0);
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

            Vec2 GetPortalLoc();
            std::string GetTileMapInfo();

        private:
            float eventduration;
            Timer* eventtimer;

            EventType eventType;
            bool processing;

            Vec2 portalLoc;
            std::string tileMapInfo;
            
    };

#endif