#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef VEC2_H
    #define VEC2_H

    class Vec2{
        public:
            Vec2();
            Vec2(float x,float y);
            Vec2& GetRotated(float R);
            float x,y;

            //ADDITION
            Vec2 operator+(const Vec2& other){
                x += other.x;
                y += other.y;

                return *this;
            }
    };
#endif