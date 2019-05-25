#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef CAMERA_H
    #define CAMERA_H
    #include "GameObject.h"
    #include "Vec2.h"

    class Camera{
        public:
            static void Follow(GameObject* newFocus);
            static void UnFollow();
            static void Update(float dt);
            static void ShakeScreen(float time,int intensity = 10);
            static void Correct();

            static Vec2 pos;
            static Vec2 speed;
            static Vec2 limit;  //Checks the limit of the current tilemap
            static Vec2 window;
        private:
            static GameObject* focus;

            static Timer* shakescreen;
            static float shaketime;
            static int shakeintensity;

    };

#endif