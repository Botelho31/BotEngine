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
            Physics* GetPhysics();

            void XMovement(float dt);   //Performs X Movement
            void YMovement(float dt);   //Performs Y Movement
            void IdleHandle(float dt); //Handles player Idle
            void SetSprite(std::string file,int framecount = 1,float frametime = 1,bool repeat = true,Vec2 offset = {0,0}); //changes the sprite in usage
            void SetCollider(float scaleX,float scaleY,float offsetX = 0,float offsetY = 0);    //changes the values of the collider

            Vec2 GetPosition();
            void DamageMinion(int damage);

            static void BiteHitbox(GameObject& hitbox,GameObject& owner,float dt);
        private:
            enum minionState{IDLE,CHASING,ATTACKING};
            minionState state;

            int hp;
            int attackrange;
            int sightrange;

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
            float damageCooldown;
            bool falling;
            bool idle;


            Physics *physics;
    };

#endif