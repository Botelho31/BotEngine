#include "../include/Resources.h"

SDL_Texture* Resources::GetImage(std::string file){
    std::unordered_map<std::string, SDL_Texture*>::iterator it;
	it = imageTable.find(file);
	if (it != imageTable.end()){
		return it->second;
	}
    else{
		SDL_Texture *texture = IMG_LoadTexture(Game::GetInstance().GetRenderer(), file.c_str());
        if(texture !=  nullptr){
            imageTable.insert({file,texture});
            return texture;
        }else{
            std::cout << "Failed to load resource: " << file << std::endl;
            return nullptr;
        }
	}
}

void Resources::ClearImages(){
    std::unordered_map<std::string, SDL_Texture*>::iterator it = imageTable.begin();

    while(it != imageTable.end()){
        it = imageTable.erase(it);
    }
    imageTable.clear();

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
        it = soundTable.erase(it);
    }
    soundTable.clear();
}