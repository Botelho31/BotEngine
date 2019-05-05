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
            MovingTile(GameObject& associated,float speed,Vec2 start,Vec2 dest);
            ~MovingTile();
            void Update(float dt);
            void Render();
            void NotifyCollision(GameObject& other);
            bool Is(std::string type);
        private:
            bool going;
            Vec2 start;
            Vec2 dest;
            Vec2 speed;
            Vec2 deltamov;
            Physics *physics;
            Sprite *tilesprite;
    };

#endif