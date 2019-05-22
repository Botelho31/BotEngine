#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef EYE_H
    #define EYE_H
    #include "Physics.h"

    class Eye : public Component{
        public:
            Eye(GameObject& associated,int radius);
            ~Eye();

            void Update(float dt);
            void Render();
            bool Is(std::string type);
            void NotifyCollision(GameObject& other);
        private:
            float radius;
            Vec2 originalpos;
            Vec2 speed;
            Physics *physics;
            
    };

#endif