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
            Physics* GetPhysics();
            void KeepStill(bool freeze,float time = 0);
            
            Vec2 GetPosition(); //Returns vector with center position of player
            float GetPlayerAngle(); //Returns angle of player in PI
            Vec2 GetSpeed(); //Returns speed on the two vectors
            int GetLife(); // Returns current hp

            void InstanceHitbox();
            void AttackHandle(float dt); // Handles player attack
            void XMovement(float dt);   //Performs X Movement
            void YMovement(float dt);   //Performs Y Movement
            void IdleHandle(float dt); //Handles player Idle
            void SpriteEffect(std::string file,int frames,float frametime,float duration,Vec2 point);
            void SetSprite(std::string file,int framecount = 1,float frametime = 1,bool repeat = true,Vec2 offset = {0,0}); //changes the sprite in usage

            void Reset(Vec2 speed = {0,0}); //Resets all player variables
            void SetInvincibility(bool Invencible);
            void SetSpeed(Vec2 speed);
            void SetHealth(int hp);
            void MovePlayer(float x,float y,bool keepMomentum = true);   //Moves the center of a player to a certain spot
            void DamagePlayer(int damage);
            void HealPlayer(int heal);
            void KnockBack(Rect hitbox,Vec2 knockback);

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
            Timer *delayedboosttimer;
            float swordarc;
            float asword;
            float aswordangle;
            float swordradius;
            float attacktiming;
            float endofattack;
            float delayedboost;
            std::queue<int> nextattack; 

            Timer *invincibilitytimer;
            int hp;
            bool invencible;
            Timer *damagetimer;

            Timer *jumpsquat;
            Timer *hittheground;
            Timer *jumpanimation;
            bool falling;

            Timer *idletimer;
            bool idle;

            Timer *runningstoptimer;
            bool running;

            Sprite *playersprite;
            InputManager *input;
            Physics *physics;
    };

#endif