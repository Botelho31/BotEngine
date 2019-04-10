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
            Vec2 GetPosition(); //Returns vector with center position of player
            Vec2 GetSpeed(); //Returns speed on the two vectors
            void SetSprite(std::string file,int framecount = 1,float frametime = 1,bool repeat = true,Vec2 offset = {0,0}); //changes the sprite in usage
            void SetCollider(float scaleX,float scaleY,float offsetX = 0,float offsetY = 0);    //changes the values of the collider
            void CorrectDistance(int distground,int distceiling,int distright,int distleft);    //Correct the distance if the player is inside a wall
            int DistanceTo(Vec2 vector1,Vec2 vector2,int xsum,int ysum);    //Gets the distance to a unpassable tile block in a direction
            bool CanMove(Vec2 vector1,Vec2 vector2);    //Checks it the side of a rect can move

            void MovePlayer(float x,float y);   //Moves the center of a player to a certain spot
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
            Timer *hittheground;
            bool falling;
            bool idle;
    };

#endif