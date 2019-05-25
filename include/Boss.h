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

            // Vec2 GetPosition();
            void DamageBoss(int damage);
            void KillBoss();
            void SpawnEye(Vec2 pos);
            // void SpawnMinion(Vec2 pos);

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
            Timer *invincibilitytimer;
            Timer *damagetimer;
            Timer *attackdelay;
            bool hitboxinstantiated;
            float damageCooldown;

            std::vector<std::weak_ptr<GameObject>> eyes; 
    };

#endif