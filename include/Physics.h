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
            void Update(int max = 150); //Checks if the object is OutofBounds or if it is colliding with a tile and adjusts accordingly
            void UpdateDists(int max = 150);    //Updates the distance in each direction (DONT USE TOO MUCH, CPU INTENSIVE)
            void CorrectDistance();    //Correct the distance if the object is inside a wall
            int DistanceTo(Rect box,int xsum,int ysum,float angle,int max = 150);    //Gets the distance to a unpassable tile block in a direction
            int SightTo(Vec2 vector,Vec2 vectorTo,int max = 150); // Gets whats the distance the line of sight reaches throught tilemap
            Rect GetLineBox(Vec2 vector,Vec2 vectorTo,float distance = 0);
            
            bool IsOutofBounds(Rect box = {0,0,0,0},float angle = 0);   //Checks if a box is Out of Bounds
            bool IsColliding(Rect box = {0,0,0,0},float angle = 0,bool nooutofbounds = false,bool markcollision = false); //Checks to see if it is colliding with tilecolliders

            //Checks if it is colliding in a direction
            bool IsRight(int sum = 1);
            bool IsUp(int sum = 1);
            bool IsLeft(int sum = 1);
            bool IsGrounded(int sum = 1);


            Vec2 GetCollisionPoint(Rect origin); //Gets the collision point on the map from the origin to the edge of hitbox, returns {0,0} if not colliding

            Vec2 Follow(Vec2 dest,float constspeed,float dt);   //Goes directly to marked point at constant speed
            float Rotate(Vec2 start,Vec2 dest,float angle,float constspeed,float dt);   //Goes in curved to the marked point at constant speed

            void PerformYAcceleration(bool increaseY,float aspeed,float dt);
            void PerformXAcceleration(bool increaseX,float aspeed,float maxspeed,float despeed,float dt);
            void PerformXDeceleration(float despeed,float dt);

            float PerformXMovement(float dt); 
            float PerformYMovement(float dt);

            void PerformGravity(float gravspeed,float dt);
            void KnockBack(Rect hitbox,Vec2 knockback); //Applies knockback to rect

            Collider* GetCollider(); //Returns the collider
            void SetCollider(float scaleX,float scaleY,float offsetX = 0,float offsetY = 0);    //changes the values of the collider

            int distground,distceiling,distright,distleft;
        private:
            Vec2 *speed;
            Collider *collider;
            GameObject *associated;
            float max; //Maximum distance that UpdateDists bases itself on
            bool isTile;    //If it is tile doesnt check for collision with map or OutofBounds
            
    };

#endif