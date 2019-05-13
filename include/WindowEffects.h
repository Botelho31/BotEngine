#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef WINDOWEFFECTS_H
    #define WINDOWEFFECTS_H
    #include "Rect.h"
    #include "Vec2.h"

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

            static void DrawBox(Rect box,float angle,int r,int g,int b);
            static void AddBoxToDraw(Rect box,float angle,int r,int g,int b);
        private:
            class BoxClass{
                public:
                BoxClass(Rect box,float angle,int r,int g,int b){
                    this->box = box;
                    this->angle = angle;
                    this->r = r;
                    this->g = g;
                    this->b = b;
                };
                Rect box;
                float angle;
                int r,g,b;
            };
            static std::vector<BoxClass> drawqueue;
            Effect currentEffect;
            float currenteffectseconds;
            float r,g,b,a;
    };

#endif