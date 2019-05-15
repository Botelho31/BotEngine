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
            Vec2& GetRotated(float R);
            void Transform(float x,float y);
            void Transform(Vec2 pos);
            Vec2 Added(float x,float y);    //Returns a new Vec with the added values
            float GetAngle(float x,float y,float offsetX = 0,float offsetY = 0);
            float GetAngle(Vec2 point,float offsetX = 0,float offsetY = 0);
            float GetDistance(float x,float y);
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

            bool operator==(const Vec2& rhs) const {
                if((x == rhs.x) && (y == rhs.y)){
                    return true;
                }else{
                    return false;
                }
            }
            bool operator!=(const Vec2& rhs) const {
                if((x != rhs.x) || (y != rhs.y)){
                    return true;
                }else{
                    return false;
                }
            }
    };
#endif