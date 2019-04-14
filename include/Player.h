#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef PLAYER_H
    #define PLAYER_H
    #include "Physics.h"

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
            int GetLife(); // Returns current hp

            void XMovement(float dt);   //Performs X Movement
            void YMovement(float dt);   //Performs Y Movement
            void IdleHandle(float dt); //Handles player Idle
            void SetSprite(std::string file,int framecount = 1,float frametime = 1,bool repeat = true,Vec2 offset = {0,0}); //changes the sprite in usage

            void SetInvincibility(bool Invencible);
            void SetSpeed(Vec2 speed);
            void KeepStill();
            void MovePlayer(float x,float y,bool keepMomentum = true);   //Moves the center of a player to a certain spot
            void DamagePlayer(int damage);
            void HealPlayer(int heal);

            static void SwordHitbox(GameObject &hitbox,GameObject& owner,float dt); //Logic for hitbox function of the sword

            static Player *player;
        private:
            InputManager *input;
            Vec2 speed;
            int maxspeed;
            int aspeed;
            int despeed;

            int gravspeed;
            int ajump;
            int awalljump;

            float swordarc;
            float asword;
            Timer *swordattack;

            int hp;
            bool invencible;
            Sprite *playersprite;
            Timer *idletimer;
            Timer *jumpsquat;
            Timer *hittheground;
            Timer *jumpanimation;
            Timer *invincibilitytimer;
            bool falling;
            bool idle;

            Physics *physics;
    };

#endif