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
            void Render(); //Render effects on to screen
            void Reset(); //Resets state to NOTHING
            void FadeToBlack(int seconds); //Sets state to fadetoblack
            void FadeFromBlack(int seconds); //Sets state to fadefromblack
            void DrawToScreen(int r,int g,int b,int a); //Draws effect to whole screen
            Effect GetCurrentEffect();  //Gets the effect currently being used
            bool Drawing(); //Returns if it is drawing or not
            bool IsBlack(); //Returns if the screen is black

            static void DrawBox(Rect box,float angle,int r,int g,int b);
            static void DrawCircle(int centreX, int centreY, int radius);
            static void FillRect(Rect box,int r,int g,int b,int a);
            static void AddBoxToDraw(Rect box,float angle,int r = 0,int g = 0,int b = 0); //Adds Box to draw Queue
        private:
            class BoxClass{ //Class that keeps a box and its color
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
            static std::vector<BoxClass> drawqueue; // Queue of boxes to be drawn to screen
            Effect currentEffect;
            float currenteffectseconds;
            float r,g,b,a;
    };

#endif