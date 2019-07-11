#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef HUD_H
    #define HUD_H
    #include "Component.h"
    #include "Physics.h"
    #include "Sprite.h"
    #include "TileSet.h"
    #include "StatBar.h"

    class HUD : public Component{
        public:
            HUD(GameObject& associated);
            ~HUD();
            void Update(float dt);
            void Render();
            bool Is(std::string type);
        private:
            GameObject* belicon;
            GameObject* healthicon;
            GameObject* manaicon;
            GameObject* manabarObj;
            GameObject* healthbarObj;
            GameObject* bossbarObj;
            StatBar* manabar;
            StatBar* healthbar;
            StatBar* bossbar;
    };

#endif