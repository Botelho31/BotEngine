#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef VEC2_H
    #define VEC2_H

    class Vec2{
        public:
            Vec2();
            Vec2(float x,float y);
            float Magnitude();
            bool Follow(float x,float y,float speedX,float speedY,float dt);
            Vec2& GetRotated(float R);
            void Transform(float x,float y);
            void Transform(Vec2 pos);
            float x,y;

            //OPERATORS
            Vec2 operator+(const Vec2& other){
                return Vec2(x + other.x,y + other.y);
            }
            Vec2 operator-(const Vec2& rhs) const {
               return Vec2(x - rhs.x, y - rhs.y);
            }

            Vec2 operator*(const float rhs) const {
               return Vec2(x * rhs, y * rhs);
            }
    };
#endif