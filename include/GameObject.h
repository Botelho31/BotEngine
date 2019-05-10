#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef GAMEOBJECT_H
    #define GAMEOBJECT_H
    #include "Rect.h"
    #include "Component.h"
    class Physics;
    class State;
    
    class GameObject{
        public:
        
            GameObject();
            ~GameObject();
            void Update(float dt);
            void Render();
            bool IsDead();
            void Start();
            void RequestDelete();
            std::weak_ptr<Component> AddComponent(Component* cpt);
            void RemoveComponent(Component* cpt);
            Component* GetComponent(std::string type);
            Physics* GetPhysics();
            void NotifyCollision (GameObject& other);
            Rect box;
            bool started;
            double angleDeg; // SENTIDO HORARIO EM GRAUS
        private:
            std::vector<std::shared_ptr<Component>> components;
            bool isDead;

    };

#endif