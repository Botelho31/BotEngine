#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef CIRCLE_H
    #define CIRCLE_H
    #include "Vec2.h"

    class Circle{
        public:
            Circle();
            Circle(int x,int y,int radius);
            Circle(Vec2 center,int radius);

            bool IsInside(int x,int y);
            bool IsInside(Vec2 pos);
            void Transform(int x,int y);
            void Transform(Vec2 pos);
            float GetDistanceFromCenter(float x,float y);
            int x,y,radius;
    };
#endif