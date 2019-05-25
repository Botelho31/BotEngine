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
            Rect(Vec2 pos,float w,float h);
            bool Contains(float x,float y);
            bool Contains(Vec2 point);
            void Transform(float x,float y);
            float Magnitude();
            float MultiplyVector(float x,float y);
            Rect Added(float x,float y,float w = 0,float h = 0);    //Returns a new Rect added to some values
            Rect Added(Rect added);
            float GetDistance(float x,float y);
            Vec2 GetOrigin();
            Vec2 GetCenter();
            void SetCenter(Vec2 center);
            float GetAngle(float x,float y,float offsetX = 0,float offsetY = 0);
            Vec2 GetMirroredPoint(Vec2 point,bool horizontal = true,bool vertical = false); //Returns the point mirrored position from the perspective of the rect
            float w,h,x,y;

            bool operator==(const Rect& rhs) const {
                if((x == rhs.x) && (y == rhs.y) && (w == rhs.w) && (h == rhs.h)){
                    return true;
                }else{
                    return false;
                }
            }
    };

#endif