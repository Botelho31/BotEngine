#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef PAUSESTATE_H
    #define PAUSESTATE_H
    #include "Timer.h"
    #include "WindowEffects.h"

    class PauseState : public State{
        public:
            PauseState();
            ~PauseState();
            void LoadAssets();
            void Update(float dt);
            void Render();

            void Start();
            void Resume();
            void Pause();
        private:
    };

#endif