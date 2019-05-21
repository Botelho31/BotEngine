#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef FAKEWALL_H
    #define FAKEWALL_H
    #include "Rect.h"
    #include "Vec2.h"
    #include "Sprite.h"

    class FakeWall : public Component{
        public:
            FakeWall(GameObject& associated,std::string wallspritefile,bool breakable = false,int hp = 100);
            ~FakeWall();

            void Update(float dt);
            void Render();
            bool Is(std::string type);
            void NotifyCollision(GameObject& other);
        private:
            enum Effect{FADE,UNFADE,STANDBY};
            Effect currentEffect;
            Sprite* wallsprite;
            float alpha;
            float alphapersecond;

            int hp;
            Timer * invincibilitytimer;
            float damageTimer;
            bool breakable;

            
    };

#endif