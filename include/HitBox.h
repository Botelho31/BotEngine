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
            HitBox(GameObject& associated,Rect hitbox,double angledeg,bool HitPlayer,bool HitEnemy);
            ~HitBox();
            void Update(float dt);
            void Render();
            bool Is(std::string type);
        private:
            bool HitPlayer;
            bool HitEnemy;
            Collider *collider;
    };

#endif