#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef PHYSICS_H
    #define PHYSICS_H
    #include "Vec2.h"
    #include "Rect.h"

    class Physics{
        public:
            Physics(GameObject* associated);
            ~Physics();
            void Update(Rect collider);
            void CorrectDistance();    //Correct the distance if the player is inside a wall
            int DistanceTo(Vec2 vector1,Vec2 vector2,int xsum,int ysum,int max = 150);    //Gets the distance to a unpassable tile block in a direction
            bool CanMove(Vec2 vector1,Vec2 vector2);    //Checks it the side of a rect can move
            int DistanceTo(Vec2 vector,Vec2 vectorTo,int max = 150); // Gets the from one vector to another throught the tilemap
            bool CanMove(Vec2 vector);    //Checks it vector can move
            void KnockBack(Rect hitbox,Vec2 *speed,float aknockbackX,float aknockbackY);
            int distground,distceiling,distright,distleft;
        private:
            GameObject *associated;
            
    };

#endif