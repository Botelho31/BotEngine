#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef PENGUINBODY_H
    #define PENGUINBODY_H
    #include "Component.h"

        class PenguinBody : public Component{
            public:
                PenguinBody(GameObject& associated);
                ~PenguinBody();

                void Start();
                void Update(float dt);
                void Render();
                bool Is(std::string type);

                static PenguinBody *player;
            private:
                std::weak_ptr<GameObject> pcannon;
                Vec2 speed;
                float linearSpeed;
                float angle;
                int hp;
        };

#endif