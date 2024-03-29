#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef TILECOLLIDER_H
    #define TILECOLLIDER_H
    #include "Rect.h"
    #include "Vec2.h"

    class TileCollider : public Component{
        public:
            TileCollider(GameObject& associated,Rect tilebox,bool moving = false,bool isMob = false);
            void Update(float dt);
            void Render();
            void Start();
            bool Is(std::string type);
            void NotifyCollision(GameObject& other);
            void NotifyMobCollision(GameObject& other);
            bool CanMove(Vec2 vector);
            float Ceiling(float number);
            bool IsMob();
            
            bool moving;
            bool isMob;
            bool adjusted;

            bool maxX;
            bool maxY;

            bool right;
            bool left;
            bool up;
            bool down;

            bool pressing;
            bool deleted;

            bool marked; // FOR DEBUGGING
            Rect box;
    };

#endif