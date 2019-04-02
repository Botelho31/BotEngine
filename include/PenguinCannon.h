#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef PENGUINCANNON_H
    #define PENGUINCANNON_H
    #include "Component.h"
    #include "Timer.h"

    class PenguinCannon : public Component{
        public:
            PenguinCannon(GameObject& associated,GameObject* penguinBody);
            ~PenguinCannon();
            void Update(float dt);
            void Render();
            bool Is(std::string);
            void NotifyCollision(GameObject& other);
            void Shoot();
        private:
            GameObject* pbody;
            float angle;
            Timer *timer;
    };

#endif