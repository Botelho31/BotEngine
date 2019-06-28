#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef BOSS_H
    #define BOSS_H
    #include "Physics.h"
    #include "Eye.h"

    class Boss : public Component{
        public:
            Boss(GameObject& associated);
            ~Boss();
            void Start();
            void Update(float dt);
            void Render();
            bool Is(std::string type);
            void NotifyCollision(GameObject& other);

            void AttackState(float dt); //Handles attack state
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

            void StopParallax();
            void CatchParallax();

            static void HandHitbox(GameObject &hitbox,GameObject& owner,float dt); //Logic for hitbox function of the hand

        private:
            enum bossState{IDLE,CHASING,ATTACKING};
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
            Timer *idlehandtimer;
            Timer *returnhandtimer;
            Timer *invincibilitytimer;
            Timer *damagetimer;
            Timer *attackdelay;
            bool hitboxinstantiated;
            float damageCooldown;

            Timer* minionspawntimer;
            std::vector<std::weak_ptr<GameObject>> eyes; 
            std::weak_ptr<GameObject> head;
    };

#endif