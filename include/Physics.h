#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef PHYSICS_H
    #define PHYSICS_H
    #include "Vec2.h"
    #include "Rect.h"
    #include "Collider.h"
    #include "TileCollider.h"

    class Physics{
        public:
            Physics(GameObject* associated,Vec2 *speed,bool isTile = false);
            ~Physics();
            void Update(int max = 150);
            void UpdateDists();
            void CorrectDistance();    //Correct the distance if the player is inside a wall
            int DistanceTo(Rect box,int xsum,int ysum,int max = 150);    //Gets the distance to a unpassable tile block in a direction
            int DistanceTo(Vec2 vector,Vec2 vectorTo,int max = 150); // Gets the from one vector to another throught the tilemap
            //NEW FUNCTIONS
            bool IsColliding(Rect box,float angle = 0); //Checks to see if it is colliding with tilecolliders
            bool IsRight(int sum = 1);
            bool IsUp(int sum = 1);
            bool IsLeft(int sum = 1);
            //END NEW FUNCTIONS

            bool IsGrounded(int sum = 1);

            Vec2 GetCollisionPoint(Rect hitbox);

            Vec2 Follow(Vec2 dest,float constspeed,float dt);   //Goes directly to marked point
            float Rotate(Vec2 start,Vec2 dest,float angle,float constspeed,float dt);   //Goes in curved to the marked point

            void PerformYAcceleration(bool increaseY,float aspeed,float dt);
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