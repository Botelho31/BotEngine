#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef PENGUINCANNON_H
    #define PENGUINCANNON_H
    #include "Component.h"
    #include "Timer.h"

    class PenguinCannon : public Component{
        public:
            PenguinCannon(GameObject& associated,std::weak_ptr<GameObject> penguinBody);
            ~PenguinCannon();
            void Update(float dt);
            void Render();
            bool Is(std::string);
            void NotifyCollision(GameObject& other);
            void Shoot();
        private:
            std::weak_ptr<GameObject> pbody;
            float angle;
            Timer *timer;
    };

#endif