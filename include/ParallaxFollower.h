#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef PARALLAXFOLLOWER_H
    #define PARALLAXFOLLOWER_H
    #include "Component.h"
    #include "Physics.h"

    class ParallaxFollower : public Component{
        public:
            ParallaxFollower(GameObject& go,float parallax);
            ~ParallaxFollower();

            void Update(float dt);
            void Render();
            bool Is(std::string type);

            void SetParallax(float parallax,bool catchParallax = false);
        private:
            Vec2 originalpos;
            float parallax;
            bool catchParallax;
            
            Physics *physics;
            Vec2 speed;
    };

#endif