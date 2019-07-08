#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef SOUND_H
    #define SOUND_H
    #include "Component.h"

    class Sound : public Component{
        public:
            Sound(GameObject& associated);
            Sound(GameObject& associated, std::string file);
            ~Sound();
            void Play (int times = 1,bool autodestruct = false);
            void Stop();
            void Open(std::string file);
            bool IsOpen();
            bool IsPlaying();

            void Update(float dt);
            void Render();
            bool Is(std::string type);
        private:
            std::shared_ptr<Mix_Chunk> chunk;
            int channel;

            bool autodestruct;
    };

#endif