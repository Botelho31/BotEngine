#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef CLOSINGDOOR_H
    #define CLOSINGDOOR_H
    #include "Rect.h"
    #include "Vec2.h"
    #include "Sprite.h"
    #include "Physics.h"

    class ClosingDoor : public Component{
        public:
            enum doorState{CLOSING,OPENING,STANDBY};
            ClosingDoor(GameObject& associated,std::string doorspritefile,Vec2 opened,Vec2 closed,doorState initialstate);
            ~ClosingDoor();

            void Update(float dt);
            void Render();
            bool Is(std::string type);
            void NotifyCollision(GameObject& other);

            void Close();
            void Open();
        private:
            doorState state;
            Vec2 opened;
            Vec2 closed;
            Vec2 speed;

            Physics *physics;

            
    };

#endif