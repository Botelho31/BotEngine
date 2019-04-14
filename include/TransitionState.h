#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef TRANSITIONSTATE_H
    #define TRANSITIONSTATE_H
    #include "Timer.h"

    class TransitionState : public State{
        public:
            TransitionState(float secondsToPop);
            ~TransitionState();
            void LoadAssets();
            void Update(float dt);
            void Render();

            void Start();
            void Resume();
            void Pause();

            void FadeToBlack(int alpha);
        private:
            Timer *poptimer;
            float secondsToPop;
            float alphapersecond;
            float currentalpha;
    };

#endif