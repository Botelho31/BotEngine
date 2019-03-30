#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef BULLET_H
    #define BULLET_H
    #include "Component.h"
    #include "Vec2.h"

    class Bullet : public Component{
        public:
            Bullet(GameObject& associated,float angle,float speed,int damage,float maxDistance,std::string sprite);
            void Update(float dt);
            void Render();
            bool Is(std::string type);
            int GetDamage();
        private:
            Vec2 speed;
            float distanceLeft;
            float angle;
            int damage;
    };

#endif