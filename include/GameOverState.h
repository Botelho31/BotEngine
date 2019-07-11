#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef GAMEOVERSTATE_H
    #define GAMEOVERSTATE_H
    #include "Timer.h"
    #include "WindowEffects.h"
    #include "Button.h"
    #include "StageState.h"

    class GameOverState : public State{
        public:
            GameOverState(StageState *stagestate);
            ~GameOverState();
            void LoadAssets();
            void Update(float dt);
            void Render();

            void Start();
            void Resume();
            void Pause();
        private:
            Button *continueButton;
            Button *returnToMenuButton;
            StageState *stagestate;
    };

#endif