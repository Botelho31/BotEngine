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

            static Mix_Music* GetMusic(std::string file);
            static void ClearMusics();

            static Mix_Chunk* GetSound(std::string file);
            static void ClearSounds();

            static TTF_Font* GetFont(std::string file,int ptsize);
            static void ClearFonts();
        private:
            static std::unordered_map<std::string, std::shared_ptr<SDL_Texture>> imageTable;
            static std::unordered_map<std::string, Mix_Music*> musicTable;
            static std::unordered_map<std::string, Mix_Chunk*> soundTable;
            static std::unordered_map<std::string, TTF_Font*> fontTable;
    };
#endif