#ifndef GAME_H
    #include "Game.h"
#endif

#ifndef RESOURCES_H
    #define RESOURCES_H

    class Resources{
        public:
            static std::shared_ptr<SDL_Texture> GetImage(std::string file);
            static void DeleteImage(SDL_Texture *texture);
            static void ClearImages();

            static std::shared_ptr<Mix_Music> GetMusic(std::string file);
            static void DeleteMusic(Mix_Music *music);
            static void ClearMusics();

            static std::shared_ptr<Mix_Chunk> GetSound(std::string file);
            static void DeleteSound(Mix_Chunk *chunk);
            static void ClearSounds();

            static std::shared_ptr<TTF_Font> GetFont(std::string file,int ptsize);
            static void DeleteFont(TTF_Font *font);
            static void ClearFonts();
        private:
            static std::unordered_map<std::string, std::shared_ptr<SDL_Texture>> imageTable;
            static std::unordered_map<std::string, std::shared_ptr<Mix_Music>> musicTable;
            static std::unordered_map<std::string, std::shared_ptr<Mix_Chunk>> soundTable;
            static std::unordered_map<std::string, std::shared_ptr<TTF_Font>> fontTable;
    };
#endif