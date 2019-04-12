#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef HITBOX_H
    #define HITBOX_H
    #include "Component.h"
    #include "Vec2.h"
    #include "Collider.h"

    class HitBox : public Component{    //COLLIDER WITHOUT NO SPRITE BEHIND IT
        public:
            HitBox(GameObject& associated,Rect hitbox,std::weak_ptr<GameObject> owner,double angledeg = 0,float secondsToSelfDestruct = 0,int damage = 0,float damageCooldownbool = 0,bool hitPlayer = true,bool hitEnemy = true,Vec2 knockback = {0,0});
            ~HitBox();
            void Update(float dt);
            void Render();
            bool Is(std::string type);
            void NotifyCollision(GameObject& other);
            
            Collider* GetCollider();
            Vec2 GetKnockBack();
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

            bool hitPlayer;
            bool hitEnemy;

            std::weak_ptr<GameObject> owner;
            
            void (*Move)(GameObject&,GameObject&,float);
    };

#endif