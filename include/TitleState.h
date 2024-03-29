#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef TITLESTATE_H
    #define TITLESTATE_H
    #include "State.h"
    #include "Timer.h"
    #include "Button.h"
    #include "Music.h"

    class TitleState : public State{
        public:
            TitleState();
            ~TitleState();
            
            void LoadAssets();
            void Update(float dt);
            void Render();

            void PlaySoundEffect(std::string file,int times = 0);

            void Start();
            void Resume();
            void Pause();
        private:
            Button *playButton;
    };

#endif