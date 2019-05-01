#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef MOVINGTILE_H
    #define MOVINGTILE_H
    #include "Component.h"
    #include "Physics.h"
    #include "Sprite.h"

    class MovingTile : public Component{
        public:
            MovingTile(GameObject& associated);
            void Update(float dt);
            void Render();
            bool Is(std::string type);
        private:
            Vec2 speed;
            Physics *physics;
            Sprite *tilesprite;
    };

#endif