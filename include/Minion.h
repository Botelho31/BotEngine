#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef MINION_H
    #define MINION_H

    class Minion : public Component{
        public:
            Minion(GameObject& associated);
            ~Minion();
            void Start();
            void Update(float dt);
            void Render();
            bool Is(std::string type);
            void NotifyCollision(GameObject& other);
        private:
            int hp;

            Vec2 speed;
            int maxspeed;
            int aspeed;
            int despeed;

            int gravspeed;

            Sprite *minionsprite;
            Timer *idletimer;
            Timer *hittheground;
            bool falling;
            bool idle;

            int distground,distceiling,distright,distleft;
    };

#endif