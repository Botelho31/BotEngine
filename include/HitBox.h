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
            HitBox(GameObject& associated,Rect hitbox,bool HitPlayer = true,float secondsToSelfDestruct = 0,bool HitEnemy = true,double angledeg = 0);
            ~HitBox();
            void Update(float dt);
            void Render();
            bool Is(std::string type);
            void NotifyCollision(GameObject& other);

            void SetFunction(void(*Move)(GameObject&));
        private:
            float secondsToSelfDestruct;
            float time;
            bool HitPlayer;
            bool HitEnemy;
            Collider *collider;
            void (*Move)(GameObject&);
    };

#endif