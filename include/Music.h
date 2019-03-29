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
            void Play(int times = -1);
            void Stop(int msToStop = 1500);
            void Open(std::string file);
            bool IsOpen();
        private:
            Mix_Music *music;
    };

#endif