#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef ALIEN_H
    #define ALIEN_H
    #include "Component.h"
    #include "Vec2.h"
    #include "Timer.h"

    class Alien : public Component{
        public:
            Alien(GameObject& associated,int nMinions);
            ~Alien();

            void Start();
            void Update(float dt);
            void Render();
            bool Is(std::string type);
            void NotifyCollision(GameObject& other);
            static int alienCount;
        private:
            Vec2 speed;
            int hp;
            int nMinions;
            enum AlienState { MOVING, RESTING };
            AlienState state;
            Timer *restTimer;
            Vec2 destination;
            std::vector<std::weak_ptr<GameObject>> minionArray;
    };

#endif