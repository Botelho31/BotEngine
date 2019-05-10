#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef TILECOLLIDER_H
    #define TILECOLLIDER_H
    #include "Rect.h"
    #include "Vec2.h"

    class TileCollider : public Component{
        public:
            TileCollider(GameObject& associated,Rect tilebox);
            void Update(float dt);
            void Render();
            void Start();
            bool Is(std::string type);
            bool max;
            Rect box;
    };

#endif