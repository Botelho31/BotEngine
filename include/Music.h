#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef MUSIC_H
    #define MUSIC_H
    #include "Resources.h"

    class Music{
        public:
            Music();
            Music(std::string file);
            ~Music();
            void Play(int times = -1,int ms = 1500);
            void Stop(int msToStop = 1500);
            void Open(std::string file);
            void Pause();
            void Resume();
            bool IsOpen();
        private:
            std::shared_ptr<Mix_Music> music;
    };

#endif