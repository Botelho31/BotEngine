#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef FLYINGMINION_H
    #define FLYINGMINION_H

    //Comonly used parameters
    #define FLYINGMINION_IDLE_FC 60
    #define FLYINGMINION_FLYING_FC 60
    #define FLYINGMINION_DAMAGE_FC 60
    #define FLYINGMINION_ATACKING_FC 60
    #define FLYINGMINION_DEADBEHIND_FC 60
    #define FLYINGMINION_DEADFRONT_FC 60

    #define FLYINGMINION_IDLE_FT 0.05
    #define FLYINGMINION_FLYING_FT 0.05
    #define FLYINGMINION_DAMAGE_FT 0.05
    #define FLYINGMINION_ATACKING_FT 0.05
    #define FLYINGMINION_DEADBEHIND_FT 0.05
    #define FLYINGMINION_DEADFRONT_FT 0.05

    #include "Physics.h"

    class FlyingMinion : public Component{
        public:
            enum minionState{IDLE,CHASING,ATTACKING,FALLINGFROMBOSS};
            FlyingMinion(GameObject& associated,minionState startingState = IDLE);
            ~FlyingMinion();
            void Start();
            void Update(float dt);
            void Render();
            bool Is(std::string type);
            void NotifyCollision(GameObject& other);


            void XMovement(float dt);   //Performs X Movement
            void YMovement(float dt);   //Performs Y Movement 
            void AttackState(float distanceToPlayer,float dt); //Handles attack state
            void IdleState(float distanceToPlayer,float dt); //Handles idle state
            void ChasingState(float distanceToPlayer,float dt); //Handles chasing state

            void IdleHandle(float dt); //Handles minion Idle

            void SetSprite(std::string file,int framecount = 1,float frametime = 1,bool repeat = true,Vec2 offset = {0,0}); //changes the sprite in usage

            minionState GetState();
            Vec2 GetPosition();
            void DamageFlyingMinion(int damage);
            void KillFlyingMinion();

            static void BiteHitbox(GameObject& hitbox,GameObject& owner,float dt);
        private:
            minionState state;

            int hp;
            int attackrange;
            int sightrange;
            float sightangle;
            Rect sightline;

            Vec2 speed;
            int maxspeed;
            int aspeed;
            int despeed;

            int gravspeed;

            std::map<std::string,std::string> spritefiles;
            Sprite *minionsprite;
            Timer *idletimer;
            Timer *hittheground;
            Timer *attacktimer;
            Timer *invincibilitytimer;
            Timer *damagetimer;
            Timer *attackdelay;
            bool hitboxinstantiated;
            bool difxpos;
            float damageCooldown;
            bool falling;
            bool idle;

            Physics *physics;
    };

#endif