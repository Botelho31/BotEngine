#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef STATE_H
    #define STATE_H
    #include "Sprite.h"
    #include "Music.h"

    class State{
        public:
            State();
            bool QuitRequested();
            void LoadAssets();
            void Update(float dt);
            void Render();

        private:
            Sprite *bg;
            Music *music;
            bool quitRequested;
    };

#endif