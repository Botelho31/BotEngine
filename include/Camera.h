#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef CAMERA_H
    #define CAMERA_H
    #include "GameObject.h"
    #include "Vec2.h"

    class Camera{
        public:
            void Follow(GameObject* newFocus);
            void UnFollow();
            void Update(float dt);

            Vec2 pos;
            Vec2 speed;
        private:
            GameObject* focus;

    };

#endif