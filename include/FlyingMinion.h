#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef FLYINGMINION_H
    #define FLYINGMINION_H

    //Comonly used parameters
    #define FLYINGMINION_IDLE_FC 30
    #define FLYINGMINION_FLYING_FC 30
    #define FLYINGMINION_DAMAGE_FC 12
    #define FLYINGMINION_ATACKING_FC 30
    #define FLYINGMINION_DEADBEHIND_FC 30
    #define FLYINGMINION_DEADFRONT_FC 30
    #define FLYINGMINION_SPITTING_FC 26

    #define FLYINGMINION_IDLE_FT 0.03
    #define FLYINGMINION_FLYING_FT 0.03
    #define FLYINGMINION_DAMAGE_FT 0.05
    #define FLYINGMINION_ATACKING_FT 0.03
    #define FLYINGMINION_DEADBEHIND_FT 0.03
    #define FLYINGMINION_DEADFRONT_FT 0.03
    #define FLYINGMINION_SPITTING_FT 0.02

    #define FLYINGMINION_SPITDELAY 1

    #include "Physics.h"

    class FlyingMinion : public Component{
        public:
            enum minionState{IDLE,SPITTING,POSITIONING};
            FlyingMinion(GameObject& associated,minionState startingState = IDLE);
            ~FlyingMinion();
            void Start();
            void Update(float dt);
            void Render();
            bool Is(std::string type);
            void NotifyCollision(GameObject& other);


            void XMovement(float dt);   //Performs X Movement
            void YMovement(float dt);   //Performs Y Movement 
            void IdleState(float dt); //Handles idle state
            void PositioningState(float dt); //Handles positioning state
            void SpittingState(float dt); //Handles spitting state

            void IdleHandle(float dt); //Handles minion Idle
            Vec2 PositioningHandle();  //Returns a direction vector to reach spitting position

            void DefineState(float distanceToPlayer);

            void CreateSpit();

            bool CheckPlayerDown();     //Check if player is in sight bellow the minion

            void SetSprite(std::string file,int framecount = 1,float frametime = 1,bool repeat = true,Vec2 offset = {0,0}); //changes the sprite in usage

            minionState GetState();
            Vec2 GetPosition();
            void DamageFlyingMinion(int damage);
            void KillFlyingMinion();


        private:
            minionState state;

            int hp;

            int sightrange;
            float sightangle;
            Rect sightline;

            int minyspit;
            int maxyspit;
            float sightanglerange[2];

            int downsightrange;
            float downsightangle;
            float downsightanglerange[2];
            Rect downsightline;

            Vec2 dirToPlayer;

            Vec2 speed;
            int maxspeed;
            int aspeed;
            int despeed;

            int gravspeed;

            std::map<std::string,std::string> spritefiles;
            Sprite *minionsprite;

            Timer *idletimer;
            Timer *spittimer;
            Timer *invincibilitytimer;
            Timer *damagetimer;
            Timer *spitdelay;
            bool hitboxinstantiated;
            float damageCooldown;
            bool idle;
            bool spitted;

            Physics *physics;
    };

#endif