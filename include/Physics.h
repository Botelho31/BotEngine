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
            Physics(GameObject* associated,Vec2 *speed,bool isTile = false);
            ~Physics();
            void Update(Rect collider,int max = 150);
            void CorrectDistance();    //Correct the distance if the player is inside a wall
            int DistanceTo(Vec2 vector1,Vec2 vector2,int xsum,int ysum,int max = 150);    //Gets the distance to a unpassable tile block in a direction
            bool CanMove(Vec2 vector1,Vec2 vector2);    //Checks it the side of a rect can move
            int DistanceTo(Vec2 vector,Vec2 vectorTo,int max = 150); // Gets the from one vector to another throught the tilemap
            bool CanMove(Vec2 vector);    //Checks it vector can move

            bool IsGrounded();

            Vec2 GetCollisionPoint(Rect hitbox);
            Vec2 Follow(Vec2 dest,float constspeed,float dt);
            void PerformXAcceleration(bool increaseX,float aspeed,float maxspeed,float despeed,float dt);
            void PerformXDeceleration(float despeed,float dt);
            float PerformXMovement(float dt);
            float PerformYMovement(float dt);
            void PerformGravity(float gravspeed,float dt);
            void KnockBack(Rect hitbox,Vec2 knockback); //Applies knockback to rect

            Collider* GetCollider();
            void SetCollider(float scaleX,float scaleY,float offsetX = 0,float offsetY = 0);    //changes the values of the collider

            int distground,distceiling,distright,distleft;
        private:
            Vec2 *speed;
            Collider *collider;
            GameObject *associated;
            float max;
            bool isTile;
            
    };

#endif