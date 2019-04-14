#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef WINDOWEFFECTS_H
    #define WINDOWEFFECTS_H

    class WindowEffects{
        public:
            WindowEffects();
            ~WindowEffects();

            void Update(float dt);
            void Render();
            void FadeToBlack(int seconds);
            void FadeFromBlack(int seconds);
            void DrawToScreen(int r,int g,int b,int a);
            bool Drawing();
        private:
            enum Effect {NOTHING,FADETOBLACK,FADEFROMBLACK};
            Effect currentEffect;
            float currenteffectseconds;
            float r,g,b,a;
    };

#endif