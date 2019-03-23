#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef GAMEOBJECT_H
    #define GAMEOBJECT_H
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
            std::vector<std::unique_ptr<Component>> components;
            bool isDead;

    };

#endif