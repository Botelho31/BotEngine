#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef SPRITE_H
    #define SPRITE_H
    #include "GameObject.h"
    #include "Component.h"
    #include "Resources.h"
    #include "Vec2.h"

    class Sprite : public Component{
        public:
            Sprite(GameObject& associated);
            Sprite(GameObject& associated,std::string file);
            ~Sprite();
            void Open(std::string file); 
            void SetClip(int x,int y,int w,int h);
            void SetScaleX(float scaleX,float scaleY);
            Vec2 GetScale();
            int GetWidth();
            int GetHeight();
            bool IsOpen();

            void Update(float dt);
            void Render();
            void Render(int x,int y);
            bool Is(std::string type);

        private:
            SDL_Texture *texture;
            int width;
            int height;
            Vec2 scale;
            SDL_Rect clip_rect;

    };

#endif