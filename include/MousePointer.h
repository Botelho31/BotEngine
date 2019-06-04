#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef MOUSEPOINTER_H
    #define MOUSEPOINTER_H
    #include "Rect.h"
    #include "Vec2.h"
    #include "Sprite.h"

    class MousePointer : public Component{
        public:
            MousePointer(GameObject& associated);
            ~MousePointer();

            void Update(float dt);
            void Render();
            bool Is(std::string type);
            void NotifyCollision(GameObject& other);

            bool IsPressed();
        private:   
    };

#endif