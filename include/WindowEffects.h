#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef WINDOWEFFECTS_H
    #define WINDOWEFFECTS_H

    class WindowEffects{
        public:
            WindowEffects();
            ~WindowEffects();
            enum Effect {NOTHING,FADETOBLACK,FADEFROMBLACK};
            void Update(float dt);
            void Render();
            void Reset();
            void FadeToBlack(int seconds);
            void FadeFromBlack(int seconds);
            void DrawToScreen(int r,int g,int b,int a);
            Effect GetCurrentEffect();
            bool Drawing();
            bool IsBlack();
        private:
            Effect currentEffect;
            float currenteffectseconds;
            float r,g,b,a;
    };

#endif