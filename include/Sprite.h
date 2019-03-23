#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef SPRITE_H
    #define SPRITE_H
    #include "GameObject.h"
    #include "Component.h"

    class Sprite : public Component{
        public:
            Sprite(GameObject& associated);
            Sprite(GameObject& associated,const char* file);
            ~Sprite();
            void Open(const char* file); 
            void SetClip(int x,int y,int w,int h);
            void Render();
            void Update(float dt);
            int GetWidth();
            int GetHeight();
            bool IsOpen();
            bool Is(std::string type);

        private:
            SDL_Texture *texture;
            int width;
            int height;
            SDL_Rect clip_rect;

    };

#endif