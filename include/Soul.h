#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef SOUL_H
    #define SOUL_H
    #include "Component.h"
    #include "Physics.h"
    #include "Sprite.h"
    #include "TileSet.h"

    class Soul : public Component{
        public:
            Soul(GameObject& associated,int soulID);
            ~Soul();
            void Update(float dt);
            void Render();
            bool Is(std::string type);
            void NotifyCollision(GameObject& other);

            void PlaySoundEffect(std::string file,int times = 0);

            void SetSprite(std::string file,int framecount = 1,float frametime = 1,bool repeat = true,Vec2 offset = {0,0});

        private:
            int soulID;
            bool catched;
            Timer *catchinganimation;
            Sprite *soulsprite;
            std::string normalsprite;
            std::string catchedsprite;
    };

#endif