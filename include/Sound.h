#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef SOUND_H
    #define SOUND_H

    class Sound : public Component{
        public:
            Sound(GameObject& associated);
            Sound(GameObject& associated, const char* file);
            ~Sound();
            void Play (int times = 1);
            void Stop();
            void Open(const char* file);
            bool IsOpen();

            void Update(float dt);
            void Render();
            bool Is(std::string type);
        private:
            Mix_Chunk* chunk;
            int channel;
    };

#endif