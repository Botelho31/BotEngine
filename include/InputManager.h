#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef INPUTMANAGER_H
    #define INPUTMANAGER_H
    class InputManager{
        public:
            InputManager& GetInstance();

            void Update();

            bool KeyPress(int key);
            bool KeyRelease(int key);
            bool IsKeyDown(int key);

            bool MousePress(int button);
            bool MouseRelease(int button);
            bool IsMouseDown(int button);

            int GetMouseX();
            int GetMouseY();

            bool QuitRequest();
        private:
            InputManager();
            ~InputManager();

            bool mouseState[6];
            int mouseUpdate[6];

            // keyState;
            // keyUpdate;

            bool quitRequested;
            int updateCouter;

            int mouseX;
            int mouseY;
    };
#endif