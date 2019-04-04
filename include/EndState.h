#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef ENDSTATE_H
    #define ENDSTATE_H
    #include "Music.h"
    #include "State.h"

    class EndState : public State{
        public:
            EndState();
            ~EndState();

            void LoadAssets();
            void Update(float dt);
            void Render();

            void Start();
            void Resume();
            void Pause();
        private:
            Music *backgroundMusic;
    };

#endif