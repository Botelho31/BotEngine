#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef PLAYER_H
    #define PLAYER_H

    class Player : public Component{
        public:
            Player(GameObject& associated);
            ~Player();

            void Start();
            void Update(float dt);
            void Render();
            bool Is(std::string type);
            void NotifyCollision(GameObject& other);
            Vec2 GetPosition();

            static Player *player;
        private:
            Vec2 speed;
            int hp;
    };

#endif;