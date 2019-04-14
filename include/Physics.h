#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef PHYSICS_H
    #define PHYSICS_H
    #include "Vec2.h"
    #include "Rect.h"
    #include "Collider.h"

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

            bool IsGrounded();

            void PerformXAcceleration(Vec2 *speed,bool increaseX,float aspeed,float maxspeed,float despeed,float dt);
            void PerformXDeceleration(Vec2 *speed,float despeed,float dt);
            void PerformXMovement(Vec2 *speed,float dt);
            void PerformYMovement(Vec2 *speed,float dt);
            void KnockBack(Rect hitbox,Vec2 *speed,Vec2 knockback); //Applies knockback to rect

            Collider* GetCollider();
            void SetCollider(float scaleX,float scaleY,float offsetX = 0,float offsetY = 0);    //changes the values of the collider
            int distground,distceiling,distright,distleft;
        private:
            Collider *collider;
            GameObject *associated;
            
    };

#endif