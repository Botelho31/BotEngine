#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef SOUND_H
    #define SOUND_H
    #include "Resources.h"
    #include "Component.h"

    class Sound : public Component{
        public:
            Sound(GameObject& associated,Resources* resources);
            Sound(GameObject& associated,Resources* resources, std::string file);
            ~Sound();
            void Play (int times = 1);
            void Stop();
            void Open(std::string file);
            bool IsOpen();
            bool IsPlaying();

            void Update(float dt);
            void Render();
            bool Is(std::string type);
        private:
            Mix_Chunk* chunk;
            Resources* resources;
            int channel;
    };

#endif