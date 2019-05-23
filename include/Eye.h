#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef EYE_H
    #define EYE_H
    #include "Physics.h"
    #include "Circle.h"

    class Eye : public Component{
        public:
            Eye(GameObject& associated,Circle bounds,int pupilradius,bool keepPupilIn = false);
            ~Eye();

            void Update(float dt);
            void Render();
            Vec2 PupilFollow(Vec2 dest,float constspeed,float dt);
            bool Is(std::string type);
            void NotifyCollision(GameObject& other);
        private:
            bool keepPupilIn;
            Circle bounds;
            Circle pupil;
            Vec2 speed;
            
    };

#endif