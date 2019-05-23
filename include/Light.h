#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef LIGHT_H
    #define LIGHT_H
    #include "Rect.h"
    #include "Vec2.h"
    #include "Circle.h"
    #include "WindowEffects.h"

    class Light{
        public:
            Light();
            ~Light();
            void Update(float dt);
            void Render(); //Render effects on to screen

            void DimScreen(int alpha);
            void SourceOfLight(Circle circle,float intensity);
        private:
            WindowEffects *windoweffects;
            std::vector<Circle> lightsources;
    };

#endif