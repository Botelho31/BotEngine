#ifndef GAME_H
    #include "Game.h"
#endif
#ifndef RECT_H
    #define RECT_H

    class Rect{
        public:
            Rect();
            Rect(float x,float y,float w,float h);
            bool Contains(float x,float y);
            void Transform(float x,float y);
            float Magnitude();
            float MultiplyVector(float x,float y);
            bool Follow(float x,float y,float speedX,float speedY,float dt);
            float w,h,x,y;
    };

#endif