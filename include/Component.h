#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef COMPONENT_H
    #define COMPONENT_H
    #include "Timer.h"
    class GameObject;

    class Component{
        public:
            Component(GameObject& associated) : associated(associated){
                freeze = false;
                freezetime = 0;
                freezetimer = new Timer();
            };
            virtual ~Component() {
                delete freezetimer;
            };
            virtual void Start() {};
            virtual void Update(float dt) = 0;
            virtual void Render() = 0;
            virtual bool Is(std::string type) = 0;
            virtual void NotifyCollision (GameObject& other) {};
            virtual void KeepStill(bool freeze,float time = 0) {
                this->freeze = freeze;
                if(time > 0){
                    freezetime = time;
                    freezetimer->Delay(0);
                }
            };

            bool GetFreeze(){
                return freeze;
            }
            void UpdateFreeze(float dt) {
                if(freezetimer->Started()){
                    freezetimer->Update(dt);
                    if(freezetimer->Get() >= freezetime){
                        freeze = false;
                        freezetime = 0;
                        freezetimer->Restart();
                    }
                }
            };
        protected:
            GameObject& associated;
            bool freeze;
            float freezetime;
            Timer *freezetimer;
    };

#endif