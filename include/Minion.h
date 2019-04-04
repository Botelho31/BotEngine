#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef MINION_H
    #define MINION_H
    #include "Component.h"
    #include "Vec2.h"

    class Minion: public Component{
        public:
            Minion(GameObject& associated,std::weak_ptr<GameObject> alienCenter,float arcOffsetDeg = 0);
            ~Minion();

            void Update(float dt);
            void Render();
            bool Is(std::string type);
            void NotifyCollision(GameObject& other);
            void Shoot(Vec2 target);
        private:
            std::weak_ptr<GameObject> alienCenter;
            float arc;
            int hp;

    };

#endif