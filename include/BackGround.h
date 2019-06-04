#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef BACKGROUND_H
    #define BACKGROUND_H
    #include "Component.h"

    class BackGround : public Component{
        public:
            BackGround(GameObject& go,std::string backgroundfile);
            
            void Update(float dt);
            void Render();
            bool Is(std::string type);

            void SetFile(std::string backgroundfile);
        private:
            Vec2 originalpos;
            Sprite* backgroundsprite;
    };

#endif