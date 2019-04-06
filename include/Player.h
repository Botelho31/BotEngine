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
            int DistanceToGround(Vec2 vector1,Vec2 vector2);
            int DistanceToCeiling(Vec2 vector1,Vec2 vector2);
            bool CanMove(Vec2 vector1,Vec2 vector2);

            static Player *player;
        private:
            Vec2 speed;
            int hp;
    };

#endif