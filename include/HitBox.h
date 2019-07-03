#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef HITBOX_H
    #define HITBOX_H
    #include "Component.h"
    #include "Vec2.h"
    #include "Physics.h"

    class HitBox : public Component{    //COLLIDER WITHOUT NO SPRITE BEHIND IT
        public:
            HitBox(GameObject& associated,Rect hitbox,std::weak_ptr<GameObject> owner,double angledeg = 0,int damage = 0,float secondsToSelfDestruct = 0,float damageCooldown = 0,bool disconnected = false,bool hitPlayer = true,bool hitEnemy = true,Vec2 knockback = {0,0},Component *component = nullptr,float hitfreezetime = 0);
            ~HitBox();
            void Update(float dt);
            void Render();
            bool Is(std::string type);
            void NotifyCollision(GameObject& other);
            void KeepStill(bool freeze,float time = 0);
            
            void HitEffect(std::string file,int frames,float frametime,float duration,Vec2 point,bool flip = false,Vec2 offset = {0,0});
            Collider* GetCollider();
            std::shared_ptr<GameObject> GetOwner();
            Vec2 GetKnockBack();
            Vec2 GetEdge(bool outer = true); //Gets outer edge of hitbox
            float GetDamageCooldown();
            int GetDamage();
            bool HitPlayer();
            bool HitEnemy();

            void SetSelfDestruct(float secondsToSelfDestruct);
            void SetDamageValues(int damage,float damageCooldown,Vec2 knockback);
            void SetHit(bool HitPlayer,bool HitEnemy);
            void SetFunction(void(*Move)(GameObject&,GameObject&,float));
        private:
            float secondsToSelfDestruct;
            Timer *selfDestruct;

            float damageCooldown;
            int damage;
            Vec2 knockback;

            float hitfreezetime;
            bool hitPlayer;
            bool hitEnemy;
            bool disconnected;

            std::weak_ptr<GameObject> owner;
            Component *component;
            Vec2 speed;
            Physics *physics;

            Vec2 cachepoint;

            std::map<std::string,std::string> spritefiles;
            void (*Move)(GameObject&,GameObject&,float);
    };

#endif