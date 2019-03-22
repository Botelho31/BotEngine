#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef MUSIC_H
    #define MUSIC_H

    class Music{
        public:
            Music();
            Music(const char* file);
            ~Music();
            void Play(int times = -1);
            void Stop(int msToStop = 1500);
            void Open(const char* file);
            bool IsOpen();
        private:
            Mix_Music *music;
    };

#endif