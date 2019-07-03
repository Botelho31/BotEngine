#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef BOSS_H
    #define BOSS_H
    #include "Physics.h"
    #include "Eye.h"

    class Boss : public Component{
        public:
            enum bossState{IDLE,CHASING,HANDATTACKING,RAMPAGEATTACKING};
            Boss(GameObject& associated);
            ~Boss();
            void Start();
            void Update(float dt);
            void Render();
            bool Is(std::string type);
            void NotifyCollision(GameObject& other);

            void HandAttackState(float dt); //Handles Hand Attack state
            void RampageAttackState(float dt);
            void IdleState(float dt); //Handles idle state
            void ChasingState(float dt); //Handles chasing state

            void IdleHandle(float dt); //Handles minion Idle

            void SetSprite(std::string file,int framecount = 1,float frametime = 1,bool repeat = true,Vec2 offset = {0,0}); //changes the sprite in usage

            void InstantiateHitBox(Rect hitbox,float duration,Vec2 knockback);
            void DamageBoss(int damage);
            void KillBoss();

            void MoveHead(Vec2 speed,float dt);
            void DestroyHead();
            void SpawnHead(Vec2 pos);
            void SpawnEye(Vec2 pos,Vec2 endpos);
            void SpawnMinion();
            void SpawnHand(Vec2 pos);
            void DestroyHand();

            void StopParallax();
            void CatchParallax();

            bossState GetState();            

            static void HandHitbox(GameObject &hitbox,GameObject& owner,float dt); //Logic for hitbox function of the hand

        private:
            bossState state;

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
            Sprite *bosssprite;

            Timer *attacktimer;
            Timer *handuptimer;
            Timer *idlehandtimer;
            Timer *returnhandtimer;
            bool lefthand,righthand;
            int rampagecount;

            Timer *invincibilitytimer;
            Timer *damagetimer;
            Timer *attackdelay;
            bool hitboxinstantiated;
            float damageCooldown;
            bool screenshake;

            Timer* minionspawntimer;
            std::vector<std::weak_ptr<GameObject>> eyes; 
            std::weak_ptr<GameObject> head;
            std::weak_ptr<GameObject> hand;
    };

#endif