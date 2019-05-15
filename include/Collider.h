#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef COLLIDER_H
    #define COLLIDER_H
    #include "Rect.h"
    #include "Vec2.h"

    class Collider : public Component{
        public:
            Collider(GameObject& associated,Vec2 scale = {1,1},Vec2 offset = {0,0});

            void Update(float dt);
            void Render();
            bool Is(std::string type);

            void SetScale(Vec2 scale);
            void SetOffSet(Vec2 offset);
            void UpdateAssociated();

            Rect box;
        private:
            Vec2 scale;
            Vec2 offset;
    };

#endif