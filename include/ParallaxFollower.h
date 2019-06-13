#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef PARALLAXFOLLOWER_H
    #define PARALLAXFOLLOWER_H
    #include "Component.h"

    class ParallaxFollower : public Component{
        public:
            ParallaxFollower(GameObject& go,float parallax);
            
            void Update(float dt);
            void Render();
            bool Is(std::string type);
        private:
            Vec2 originalpos;
            float parallax;
    };

#endif