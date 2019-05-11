#ifndef GAME_H
    #include "Game.h"
#endif
#ifndef RECT_H
    #define RECT_H
    #include "Vec2.h"

    class Rect{
        public:
            Rect();
            Rect(float x,float y,float w,float h);
            bool Contains(float x,float y);
            void Transform(float x,float y);
            float Magnitude();
            float MultiplyVector(float x,float y);
            Rect Added(float x,float y,float w = 0,float h = 0);    //Returns a new Rect added to some values
            Rect Added(Rect added);
            float GetDistance(float x,float y);
            Vec2 GetCenter();
            float GetAngle(float x,float y,float offsetX = 0,float offsetY = 0);
            // bool Follow(float x,float y,float speedX,float speedY,float dt);
            float w,h,x,y;
    };

#endif