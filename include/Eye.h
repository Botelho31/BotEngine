#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef EYE_H
    #define EYE_H
    #include "Physics.h"
    #include "Circle.h"

    class Eye : public Component{
        public:
            Eye(GameObject& associated,Circle bounds,Vec2 end,int pupilradius,bool keepPupilIn = false);
            ~Eye();

            void Start();
            void Update(float dt);
            void Render();
            Vec2 PupilFollow(Vec2 dest,float constspeed,float dt);
            bool Is(std::string type);
            void NotifyCollision(GameObject& other);

            void SpawnMinion();
            void SetParallax(float value);
            
            void SetOriginalPoint(float addX,float addY);
            bool GoToEndPoint(float constspeed,float dt);
            bool GoToStartPoint(float constspeed,float dt);
            bool Follow(Vec2 pos,float constspeed,float dt);
        private:
            bool keepPupilIn;
            Circle bounds;
            Circle pupil;
            Vec2 speed;

            float parallaxvalue;
            Vec2 originalorigin;
            Vec2 start;
            Vec2 end;

            Sprite *eyelid;
            
    };

#endif