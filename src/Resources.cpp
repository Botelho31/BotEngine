#include "../include/Resources.h"

std::unordered_map<std::string, std::shared_ptr<SDL_Texture>> Resources::imageTable;
std::unordered_map<std::string, std::shared_ptr<Mix_Music>> Resources::musicTable;
std::unordered_map<std::string, std::shared_ptr<Mix_Chunk>> Resources::soundTable;
std::unordered_map<std::string, std::shared_ptr<TTF_Font>> Resources::fontTable;

std::shared_ptr<SDL_Texture> Resources::GetImage(std::string file){
    std::unordered_map<std::string, std::shared_ptr<SDL_Texture>>::iterator it;
	it = imageTable.find(file);
	if (it != imageTable.end()){
		return it->second;
	}
    else{
		SDL_Texture *texture = IMG_LoadTexture(Game::GetInstance().GetRenderer(), file.c_str());
        if(texture !=  nullptr){
            std::shared_ptr<SDL_Texture> textptr(texture,DeleteImage);
            imageTable.insert({file,textptr});
            return textptr;
        }else{
            std::cout << "Failed to load resource: " << file << std::endl;
            return nullptr;
        }
	}
}

void Resources::DeleteImage(SDL_Texture *texture){
    SDL_DestroyTexture(texture);
}

void Resources::ClearImages(){
    std::unordered_map<std::string, std::shared_ptr<SDL_Texture>>::iterator it = imageTable.begin();

    while(it != imageTable.end()){
        if(it->second.unique()){
            it = imageTable.erase(it);
        }else{
            it++;
        }
    }

}

std::shared_ptr<Mix_Music> Resources::GetMusic(std::string file){
    std::unordered_map<std::string, std::shared_ptr<Mix_Music>>::iterator it;
	it = musicTable.find(file);
	if (it != musicTable.end()){
		return it->second;
	}
    else{
		Mix_Music *music = Mix_LoadMUS(file.c_str());
        if(music !=  nullptr){
            std::shared_ptr<Mix_Music> musicptr(music,DeleteMusic);
            musicTable.insert({file,musicptr});
            return musicptr;
        }else{
            std::cout << "Failed to load resource: " << file << std::endl;
            return nullptr;
        }
	}
}

void Resources::DeleteMusic(Mix_Music *music){
    Mix_FreeMusic(music);
}

void Resources::ClearMusics(){
    std::unordered_map<std::string, std::shared_ptr<Mix_Music>>::iterator it = musicTable.begin();

    while(it != musicTable.end()){
        it = musicTable.erase(it);
    }
    musicTable.clear();
}

std::shared_ptr<Mix_Chunk> Resources::GetSound(std::string file){
    std::unordered_map<std::string, std::shared_ptr<Mix_Chunk>>::iterator it;
	it = soundTable.find(file);
	if (it != soundTable.end()){
		return it->second;
	}
    else{
		Mix_Chunk* chunk = Mix_LoadWAV(file.c_str());
        if(chunk !=  nullptr){
            std::shared_ptr<Mix_Chunk> chunkptr (chunk,DeleteSound);
            soundTable.insert({file,chunkptr});
            return chunkptr;
        }else{
            std::cout << "Failed to load resource: " << file << std::endl;
            return nullptr;
        }
	}
}

void Resources::DeleteSound(Mix_Chunk *chunk){
    Mix_FreeChunk(chunk);
}

void Resources::ClearSounds(){
    std::unordered_map<std::string, std::shared_ptr<Mix_Chunk>>::iterator it = soundTable.begin();

    while(it != soundTable.end()){
        it = soundTable.erase(it);
    }
    soundTable.clear();
}

std::shared_ptr<TTF_Font> Resources::GetFont(std::string file,int ptsize){
    std::unordered_map<std::string, std::shared_ptr<TTF_Font>>::iterator it;
    std::stringstream key;
    key << file << ptsize;
	it = fontTable.find(key.str());
	if (it != fontTable.end()){
		return it->second;
	}
    else{
		TTF_Font* font = TTF_OpenFont(file.c_str(),ptsize);
        if(font){
            std::shared_ptr<TTF_Font> fontptr (font,DeleteFont);
            fontTable.insert({key.str(),fontptr});
            return fontptr;
        }else{
            std::cout << "Failed to load resource: " << file << std::endl;
            return nullptr;
        }
	}
}

void Resources::DeleteFont(TTF_Font *font){
    TTF_CloseFont(font);
}

void Resources::ClearFonts(){
    std::unordered_map<std::string, std::shared_ptr<TTF_Font>>::iterator it = fontTable.begin();

    while(it != fontTable.end()){
        it = fontTable.erase(it);
    }
    fontTable.clear();
}