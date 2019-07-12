#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef PAUSESTATE_H
    #define PAUSESTATE_H
    #include "Timer.h"
    #include "WindowEffects.h"
    #include "Button.h"
    #include "StageState.h"

    class PauseState : public State{
        public:
            PauseState(StageState *stagestate);
            ~PauseState();
            void LoadAssets();
            void Update(float dt);
            void Render();

            void PlaySoundEffect(std::string file,int times = 0);

            void Start();
            void Resume();
            void Pause();
        private:
            Button *continueButton;
            Button *returnToMenuButton;
            GameObject *screenObj;
            StageState *stagestate;
    };

#endif