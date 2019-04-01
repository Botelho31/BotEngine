#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef TIMER_H
    #define TIMER_H

    class Timer{   
        public:
            Timer();
            void Update(float dt);
            void Restart();
            float Get();
        private:
            float time;
    };

#endif