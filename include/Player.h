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

            void InstanceHitbox(float asword,float swordarc);
            void AttackHandle(float dt); // Handles player attack
            void XMovement(float dt);   //Performs X Movement
            void YMovement(float dt);   //Performs Y Movement
            void IdleHandle(float dt); //Handles player Idle
            void SetSprite(std::string file,int framecount = 1,float frametime = 1,bool repeat = true,Vec2 offset = {0,0}); //changes the sprite in usage

            void SetInvincibility(bool Invencible);
            void SetSpeed(Vec2 speed);
            void KeepStill(Vec2 FreezePos);
            void MovePlayer(float x,float y,bool keepMomentum = true);   //Moves the center of a player to a certain spot
            void DamagePlayer(int damage);
            void HealPlayer(int heal);

            static void SwordHitbox(GameObject &hitbox,GameObject& owner,float dt); //Logic for hitbox function of the sword

            static Player *player;
        private:
            Vec2 speed;
            int maxspeed;
            int aspeed;
            int despeed;

            int gravspeed;
            int ajump;
            int awalljump;

            Timer *swordattack;
            float swordarc;
            float asword;
            int currentAttack;
            float attacktiming;
            float endofattack;
            std::queue<int> nextattack; 

            Timer *invincibilitytimer;
            int hp;
            bool invencible;

            Timer *jumpsquat;
            Timer *hittheground;
            Timer *jumpanimation;
            bool falling;

            Timer *idletimer;
            bool idle;

            Timer *runningstarttimer;
            Timer *runningstoptimer;
            bool running;

            Sprite *playersprite;
            InputManager *input;
            Physics *physics;
    };

#endif