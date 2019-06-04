#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef BUTTON_H
    #define BUTTON_H
    #include "Rect.h"
    #include "Vec2.h"
    #include "Sprite.h"

    class Button : public Component{
        public:
            enum buttonState{HIGHLIGHTED,SELECTED,NORMAL};
            Button(GameObject& associated,Vec2 center,std::string normalfile,std::string highlightedfile,std::string selectedfile);
            ~Button();

            void Update(float dt);
            void Render();
            bool Is(std::string type);
            void NotifyCollision(GameObject& other);

            void SetSprite(std::string file,int framecount = 1,float frametime = 1,bool repeat = true,Vec2 offset = {0,0});
        private:
            buttonState state;
            Sprite *buttonsprite;
            std::string normalfile;
            std::string highlightedfile;
            std::string selectedfile; 
            bool colliding;   
    };

#endif