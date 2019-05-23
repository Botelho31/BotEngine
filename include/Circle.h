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

            Circle Added(int x,int y,int radius = 0);
            bool IsInside(int x,int y);
            bool IsInside(Vec2 pos);
            bool IsInside(Circle circle);
            void Transform(int x,int y);
            void Transform(Vec2 pos);
            Vec2 GetCenter();
            float GetDistanceFromCenter(float x,float y);
            float GetAngleFromCenter(float x,float y);
            float x,y;
            int radius;
    };
#endif