#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef COMPONENT_H
    #define COMPONENT_H
    #include "Timer.h"
    class GameObject;
    class Physics;

    class Component{
        public:
            Component(GameObject& associated);
            virtual ~Component();
            virtual void Start() {};
            virtual void Update(float dt) = 0;
            virtual void Render() = 0;
            virtual bool Is(std::string type) = 0;
            
            virtual void NotifyCollision (GameObject& other) {};
            virtual void KeepStill(bool freeze,float time = 0);
            virtual bool GetFreeze();
            virtual void UpdateFreeze(float dt);
        protected:
            GameObject& associated;
            bool freeze;
            float freezetime;
            Timer *freezetimer;
    };

#endif