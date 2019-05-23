#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef EYE_H
    #define EYE_H
    #include "Physics.h"
    #include "Circle.h"

    class Eye : public Component{
        public:
            Eye(GameObject& associated,Circle bounds,int pupilradius);
            ~Eye();

            void Update(float dt);
            void Render();
            bool Is(std::string type);
            void NotifyCollision(GameObject& other);
        private:
            Circle bounds;
            Circle pupil;
            Vec2 speed;
            Physics *physics;
            
    };

#endif