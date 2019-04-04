#include "../include/Resources.h"

std::unordered_map<std::string, std::shared_ptr<SDL_Texture>> Resources::imageTable;
std::unordered_map<std::string, Mix_Music*> Resources::musicTable;
std::unordered_map<std::string, Mix_Chunk*> Resources::soundTable;
std::unordered_map<std::string, TTF_Font*> Resources::fontTable;

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

Mix_Music* Resources::GetMusic(std::string file){
    std::unordered_map<std::string, Mix_Music*>::iterator it;
	it = musicTable.find(file);
	if (it != musicTable.end()){
		return it->second;
	}
    else{
		Mix_Music *music = Mix_LoadMUS(file.c_str());
        if(music !=  nullptr){
            musicTable.insert({file,music});
            return music;
        }else{
            std::cout << "Failed to load resource: " << file << std::endl;
            return nullptr;
        }
	}
}

void Resources::ClearMusics(){
    std::unordered_map<std::string, Mix_Music*>::iterator it = musicTable.begin();

    while(it != musicTable.end()){
        Mix_FreeMusic(it->second);
        it = musicTable.erase(it);
    }
    musicTable.clear();
}

Mix_Chunk* Resources::GetSound(std::string file){
    std::unordered_map<std::string, Mix_Chunk*>::iterator it;
	it = soundTable.find(file);
	if (it != soundTable.end()){
		return it->second;
	}
    else{
		Mix_Chunk* chunk = Mix_LoadWAV(file.c_str());
        if(chunk !=  nullptr){
            soundTable.insert({file,chunk});
            return chunk;
        }else{
            std::cout << "Failed to load resource: " << file << std::endl;
            return nullptr;
        }
	}
}

void Resources::ClearSounds(){
    std::unordered_map<std::string, Mix_Chunk*>::iterator it = soundTable.begin();

    while(it != soundTable.end()){
        Mix_FreeChunk(it->second);
        it = soundTable.erase(it);
    }
    soundTable.clear();
}

TTF_Font* Resources::GetFont(std::string file,int ptsize){
    std::unordered_map<std::string, TTF_Font*>::iterator it;
    std::stringstream key;
    key << file << ptsize;
	it = fontTable.find(key.str());
	if (it != fontTable.end()){
		return it->second;
	}
    else{
		TTF_Font* font = TTF_OpenFont(file.c_str(),ptsize);
        if(font){
            fontTable.insert({key.str(),font});
            return font;
        }else{
            std::cout << "Failed to load resource: " << file << std::endl;
            return nullptr;
        }
	}
}

void Resources::ClearFonts(){
    std::unordered_map<std::string, TTF_Font*>::iterator it = fontTable.begin();

    while(it != fontTable.end()){
        TTF_CloseFont(it->second);
        it = fontTable.erase(it);
    }
    fontTable.clear();
}