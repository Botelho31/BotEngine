#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef BACKGROUND_H
    #define BACKGROUND_H
    #include "Component.h"

    class BackGround : public Component{
        public:
            BackGround(GameObject& go,std::string backgroundfile,bool parallax = true,bool camerafollower = false);
            
            void Update(float dt);
            void Render();
            bool Is(std::string type);

            void ChangeBackground(std::string backgroundfile,bool parallax = true,Vec2 scale = {1,1},bool camerafollower = false);
        private:
            Vec2 originalpos;
            Sprite* backgroundsprite;
            bool parallax;
            bool camerafollower;
    };

#endif