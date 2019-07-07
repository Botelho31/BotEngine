#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef MUSIC_H
    #define MUSIC_H
    #include "Resources.h"
    #include "Timer.h"

    class Music{
        public:
            Music();
            Music(std::string file);
            ~Music();
            void Update(float dt);
            void Play(int times = -1,int ms = 1500,bool maintainPos = false);
            void Stop(int msToStop = 1500);
            void Open(std::string file);
            void Pause();
            void Resume();
            float GetElapsedTime();
            bool IsOpen();
        private:
            std::shared_ptr<Mix_Music> music;

            Timer *timer;
            bool playing;
    };

#endif