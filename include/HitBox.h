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
            HitBox(GameObject& associated,std::weak_ptr<GameObject> owner,Rect hitbox,int damage,float damageCooldownbool,bool HitPlayer = true,bool HitEnemy = true,float secondsToSelfDestruct = 0,double angledeg = 0);
            ~HitBox();
            void Update(float dt);
            void Render();
            bool Is(std::string type);
            void NotifyCollision(GameObject& other);

            void SetFunction(void(*Move)(GameObject&,GameObject&,float));
        private:
            float secondsToSelfDestruct;
            Timer *selfDestruct;

            float damageCooldown;
            int damage;

            bool HitPlayer;
            bool HitEnemy;

            std::weak_ptr<GameObject> owner;
            
            void (*Move)(GameObject&,GameObject&,float);
    };

#endif