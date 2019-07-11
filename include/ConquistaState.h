#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef CONQUISTASTATE_H
    #define CONQUISTASTATE_H
    #include "Timer.h"
    #include "WindowEffects.h"
    #include "Button.h"
    #include "StageState.h"

    class ConquistaState : public State{
        public:
            ConquistaState(StageState *stagestate,std::string file,Vec2 pos);
            ~ConquistaState();
            void LoadAssets();
            void Update(float dt);
            void Render();

            void Start();
            void Resume();
            void Pause();
        private:
            StageState *stagestate;
    };

#endif