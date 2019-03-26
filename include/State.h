#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef STATE_H
    #define STATE_H
    #include "Sprite.h"
    #include "Music.h"
    #include "GameObject.h"

    class State{
        public:
            State(Resources* resources);
            ~State();
            void AddObject(int mouseX,int mouseY);
            bool QuitRequested();
            void LoadAssets();
            void Update(float dt);
            void Render();

        private:
            Sprite *bg;
            Music *music;
            Resources *resources;
            bool quitRequested;
            std::vector<std::unique_ptr<GameObject>> objectArray;
    };

#endif