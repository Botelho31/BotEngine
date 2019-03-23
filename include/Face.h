#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef FACE_H
    #define FACE_H
    
    class Face : public Component{
        public:
            Face(GameObject& associated);
            void Damage(int damage);

            void Update(float dt);
            void Render();
            bool Is(std::string type);
        private:
            int hitpoints;
    };

#endif