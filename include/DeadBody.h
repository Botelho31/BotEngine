#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef DEADBODY_H
    #define DEADBODY_H
    #include "Physics.h"

    class DeadBody : public Component{
        public:
            DeadBody(GameObject& associated,Vec2 dyingspeed,Sprite *dyingsprite,Vec2 colliderscale,Vec2 collideroffset = {0,0});
            ~DeadBody();
            void Start();
            void Update(float dt);
            void Render();
            bool Is(std::string type);
            void NotifyCollision(GameObject& other);
            Physics* GetPhysics();

            void IdleHandle(float dt);

            void SetSprite(std::string file,int framecount = 1,float frametime = 1,bool repeat = true,Vec2 offset = {0,0}); //changes the sprite in usage

            Vec2 GetPosition();
        private:
            Vec2 speed;

            int gravspeed;
            int despeed;

            Timer *invincibilitytimer;
            float damageCooldown;

            Timer *idletimer;
            bool idle;

            Sprite *deadbodysprite;
            Physics *physics;
    };

#endif