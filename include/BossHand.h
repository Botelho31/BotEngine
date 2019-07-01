#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef BOSSHAND_H
    #define BOSSHAND_H
    #include "Component.h"
    #include "Physics.h"
    #include "Sprite.h"
    #include "TileSet.h"

    class BossHand : public Component{
        public:
            BossHand(GameObject& associated,std::weak_ptr<GameObject> boss,Vec2 pos);
            ~BossHand();
            void Update(float dt);
            void Render();
            bool Is(std::string type);
            void NotifyCollision(GameObject& other);
        private:
            std::weak_ptr<GameObject> boss;
    };

#endif