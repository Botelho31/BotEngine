#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef BOSS_H
    #define BOSS_H
    #include "Physics.h"
    #include "Eye.h"

    class Boss : public Component{
        public:
            enum bossState{IDLE,CHASING,HANDATTACKING,RAMPAGEATTACKING,APPEARING,DYING,BITING};
            Boss(GameObject& associated);
            ~Boss();
            void Start();
            void Update(float dt);
            void Render();
            bool Is(std::string type);
            void NotifyCollision(GameObject& other);

            void HandAttackState(float dt); //Handles Hand Attack state
            void BiteState(float dt); //Handles Hand Attack state
            void RampageAttackState(float dt);
            void IdleState(float dt); //Handles idle state
            void ChasingState(float dt); //Handles chasing state
            void AppearingState(float dt); //Handles appearing state

            void IdleHandle(float dt); //Handles minion Idle

            void SetSprite(std::string file,int framecount = 1,float frametime = 1,bool repeat = true,Vec2 offset = {0,0}); //changes the sprite in usage
            void HitEffect(std::string file,int frames,float frametime,float duration,Vec2 point,bool flip = false,Vec2 offset = {0,0});
             void PlaySoundEffect(std::string file,int times = 0);

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

            void SpawnHeadHitbox(Vec2 pos);
            void DestroyHeadHitbox();

            void SpeedUpParallax();

            void StopParallax();
            void CatchParallax();

            bossState GetState();    
            int GetHP();        

            static void HandHitbox(GameObject &hitbox,GameObject& owner,float dt); //Logic for hitbox function of the hand

            static Boss *boss;
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

            bool bossrisehalf;

            std::map<std::string,std::string> spritefiles;
            Sprite *bosssprite;

            Timer *bossrisetimer;
            Timer *bossdeathtimer;
            Timer *attacktimer;
            Timer *appearinganimtimer;
            Timer *appearingtimer;
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

            std::weak_ptr<GameObject> headhitbox;
            std::weak_ptr<GameObject> hand;
    };

#endif