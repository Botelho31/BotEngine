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

            bool Follow(Vec2 dest,float constspeed,float dt);
            void AddOriginalPos(Vec2 add);
            void SetParallax(float parallax,bool catchParallax = false);
            void Jump();
        private:
            Vec2 originalpos;
            float parallax;
            bool catchParallax;
        
            Vec2 speed;
    };

#endif