#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef ROTATING_H
    #define ROTATING_H
    #include "Component.h"
    #include "Physics.h"
    #include "Sprite.h"
    #include "TileSet.h"

    class Rotating : public Component{
        public:
            Rotating(GameObject& associated,int rotatingSum);
            ~Rotating();
            void Update(float dt);
            void Render();
            bool Is(std::string type);
        private:
            int rotatingSum;
    };

#endif