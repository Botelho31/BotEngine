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
            MovingTile(GameObject& associated,float speed,Vec2 start,Vec2 dest,bool circular);
            ~MovingTile();
            void Update(float dt);
            void Render();
            void NotifyCollision(GameObject& other);
            bool Is(std::string type);

            void MoveObject(GameObject& other,Vec2 deltamov);
            void InvertDirection();
        private:
            bool going;
            bool circular;
            float constspeed;
            float angle;
            Vec2 start;
            Vec2 dest;
            Vec2 speed;

            std::queue<std::weak_ptr<GameObject>> movingList;
            TileCollider *tilecollider;
            Physics *physics;
            Sprite *tilesprite;
    };

#endif