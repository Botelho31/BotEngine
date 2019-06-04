#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef SPIKE_H
    #define SPIKE_H
    #include "Component.h"
    #include "Physics.h"
    #include "Sprite.h"
    #include "TileSet.h"

    class Spike : public Component{
        public:
            Spike(GameObject& associated,Vec2 knockback);
            ~Spike();
            void Update(float dt);
            void Render();
            bool Is(std::string type);

            Vec2 GetKnockback();
            int GetDamage();
        private:
            Vec2 knockback;
            int damage;
            TileSet *spiketileset;
    };

#endif