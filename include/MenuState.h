#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef MENUSTATE_H
    #define MENUSTATE_H
    #include "State.h"
    #include "Timer.h"
    #include "Button.h"
    #include "WindowEffects.h"

    class MenuState : public State{
        public:
            MenuState();
            ~MenuState();
            
            void LoadAssets();
            void Update(float dt);
            void Render();

            void Start();
            void Resume();
            void Pause();
        private:
            Button *newgameButton;
            Button *loadButton;
            Button *optionsButton;
            Button *quitButton;
            Button *infoButton;
            WindowEffects *windoweffects;
    };

#endif