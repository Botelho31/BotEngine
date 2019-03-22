#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef COMPONENT_H
    #define COMPONENT_H
    #include "GameObject.h"

    class Component{
        public:
            Component(GameObject& associated);
            virtual ~Component();
            virtual void Update(float dt) = 0;
            virtual void Render() = 0;
            virtual bool Is(std::string type) = 0;
        protected:
            GameObject& associated;
    };

#endif