#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef PENGUINCANNON_H
    #define PENGUINCANNON_H
    #include "Component.h"

    class PenguinCannon : public Component{
        public:
            PenguinCannon(GameObject& associated,GameObject* penguinBody);
            void Update(float dt);
            void Render();
            bool Is(std::string);
            void Shoot();
        private:
            GameObject* pbody;
            float angle;
    };

#endif