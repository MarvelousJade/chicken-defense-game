#ifndef ATLAS_H
#define ATLAS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <SDL2/SDL_render.h>
#include <vector>
#include <string>
#include <stdio.h>

class Atlas {
    std::vector<SDL_Texture*> textureList; 
public:
    Atlas() = default;

    ~Atlas() {
        for (SDL_Texture* texture : textureList) {
            SDL_DestroyTexture(texture);
        }
    };

    void load(SDL_Renderer* renderer, const char* pathTemplate, int num) {
        for (int i = 0; i < num; i++) {
            char pathFile[255];
            std::snprintf(pathFile, sizeof(pathFile), pathTemplate, i + 1);
            SDL_Texture* texture = IMG_LoadTexture(renderer, pathFile);
            textureList.push_back(texture);
        }
    }

    void clear() {
        textureList.clear();
    }

    int getSize() const {
        return (int)textureList.size();
    }

    SDL_Texture* getTexture(int idx) const {
        if (idx < 0 || idx >= textureList.size()) return nullptr;

        return textureList[idx];
    }

    void addTexture(SDL_Texture* texture) {
        textureList.push_back(texture);
    }
};

#endif
