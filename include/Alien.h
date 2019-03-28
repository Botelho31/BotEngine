#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef ALIEN_H
    #define ALIEN_H
    #include "Component.h"
    #include "Vec2.h"

    class Alien : public Component{
        public:
            Alien(GameObject& associated,Resources* resources,int nMinions);
            ~Alien();

            void Start();
            void Update(float dt);
            void Render();
            bool Is(std::string type);
        private:
            class Action{
                public:
                    enum ActionType {MOVE,SHOOT};
                    Action(Action::ActionType type,float x,float y);
                    ActionType type;
                    Vec2 pos;
            };
            Vec2 speed;
            int hp;
            std::queue<Action>  taskQueue;
            std::vector<std::weak_ptr<GameObject>> minionArray;
    };

#endif