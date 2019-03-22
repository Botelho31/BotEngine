#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef GAMEOBJECT_H
    #define GAMEOBJECT_H
    #include "Component.h"
    #include "Rect.h"
    class Component;
    class GameObject{
        public:
            GameObject();
            ~GameObject();
            void Update(float dt);
            void Render();
            bool IsDead();
            void RequestDelete();
            void AddComponent(Component* cpt);
            void RemoveComponent(Component* cpt);
            Component* GetComponent(std::string type);
            Rect box;
        private:
            std::vector <Component*> components;
            bool isDead;

    };

#endif