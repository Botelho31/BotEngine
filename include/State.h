#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef STATE_H
    #define STATE_H
    #include "Sprite.h"
    #include "Music.h"
    #include "GameObject.h"
    #include "State.h"

    class State{
        public:
            State();
            ~State();
            // void AddObject(int mouseX,int mouseY);
            void Start();
            std::weak_ptr<GameObject> AddObject(GameObject *go);
            std::weak_ptr<GameObject> GetObjectPtr(GameObject *go);
            bool QuitRequested();
            void LoadAssets();
            void Update(float dt);
            void Render();

        private:
            Sprite *bg;
            Music *music;
            bool quitRequested;
            bool started;
            std::vector<std::shared_ptr<GameObject>> objectArray;
    };

#endif