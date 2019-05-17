#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef MINION_H
    #define MINION_H
    #include "Physics.h"

    class Minion : public Component{
        public:
            Minion(GameObject& associated);
            ~Minion();
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

            Vec2 GetPosition();
            void DamageMinion(int damage);
            void KillMinion();

            static void BiteHitbox(GameObject& hitbox,GameObject& owner,float dt);
        private:
            enum minionState{IDLE,CHASING,ATTACKING};
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