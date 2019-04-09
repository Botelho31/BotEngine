#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef PLAYER_H
    #define PLAYER_H

    class Player : public Component{
        public:
            Player(GameObject& associated);
            ~Player();

            void Start();
            void Update(float dt);
            void Render();
            bool Is(std::string type);
            void NotifyCollision(GameObject& other);
            Vec2 GetPosition();
            void SetSprite(std::string file,int framecount = 1,float frametime = 1,bool repeat = true);
            void SetCollider(float scaleX,float scaleY,float offsetX = 0,float offsetY = 0);
            void CorrectDistance(int distground,int distceiling,int distright,int distleft);
            int DistanceTo(Vec2 vector1,Vec2 vector2,int xsum,int ysum);
            int DistanceToGround(Vec2 vector1,Vec2 vector2);
            int DistanceToCeiling(Vec2 vector1,Vec2 vector2);
            bool CanMove(Vec2 vector1,Vec2 vector2);

            void MovePlayer(float x,float y);
            static Player *player;
        private:
            Vec2 speed;
            int maxspeed;
            int aspeed;
            int despeed;

            int gravspeed;
            int ajump;
            int awalljump;

            int hp;
            Sprite *playersprite;
            Timer *idletimer;
            Timer *jumpsquat;
            bool falling;
            bool idle;
    };

#endif