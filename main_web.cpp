#define SDL_MAIN_HANDLED

#include "Atlas.h"
#include "Camera.h"
#include "Bullet.h"
#include "Chicken.h"
#include "ChickenFast.h"
#include "ChickenMedium.h"
#include "ChickenSlow.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif

#include <chrono>
#include <thread>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

Camera* camera = nullptr;

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

bool isRunning = true;

SDL_Texture* texHeart = nullptr;
SDL_Texture* g_texBullet = nullptr;
SDL_Texture* texBattery = nullptr;
SDL_Texture* texCrosshair = nullptr;
SDL_Texture* texBackground = nullptr;
SDL_Texture* texBarrelIdle = nullptr;

Atlas atlasBarrelFire;
Atlas g_atlasChickenFast;
Atlas g_atlasChickenMedium;
Atlas g_atlasChickenSlow;
Atlas g_atlasExplosion;

Mix_Music* musicBgm = nullptr;
Mix_Music* musicLoss = nullptr;

Mix_Chunk* soundHurt = nullptr;
Mix_Chunk* soundFire1 = nullptr;
Mix_Chunk* soundFire2 = nullptr;
Mix_Chunk* soundFire3 = nullptr;
Mix_Chunk* g_soundExplosion = nullptr;

TTF_Font* font = nullptr;

int hp = 10;
int score = 0;
std::vector<Bullet> bulletList;
std::vector<Chicken*> chickenList;

int numPerGen = 2;
Timer timerGenerate;
Timer timerIncreaseNumPerGen;

Vector2 posCrosshair;
double angleBarrel = 0;
const Vector2 posBattery = { 630, 600 };
const Vector2 posBarrel = { 592, 585 };
const SDL_FPoint centerBarrel = { 48, 25 };

bool isCooldown = true;
bool isFireKeyDown = false;
Animation animationBarrelFire;

// Global variables for frame timing
std::chrono::steady_clock::time_point lastTick = std::chrono::steady_clock::now();

void loadResources();
void unloadResources();
void init();
void clean();
void update(float deltaTime);
void render(const Camera& camera);
void mainLoop();

// Emscripten main loop function
void emscripten_main_loop() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                isRunning = false;
                #ifdef __EMSCRIPTEN__
                emscripten_cancel_main_loop();
                #endif
                break;
            case SDL_MOUSEMOTION: {
                posCrosshair.x = (float)event.motion.x;
                posCrosshair.y = (float)event.motion.y;
                Vector2 direction = posCrosshair - posBarrel;
                angleBarrel = std::atan2(direction.y, direction.x) * 180 / M_PI;
            }
            break;
            case SDL_MOUSEBUTTONDOWN:
                isFireKeyDown = true;
                break;
            case SDL_MOUSEBUTTONUP:
                isFireKeyDown = false;
                break;
        }
    }
    
    auto frameStart = std::chrono::steady_clock::now();
    auto deltaTime = std::chrono::duration<float>(frameStart - lastTick).count();
    
    update(deltaTime);
    render(*camera);
    
    SDL_RenderPresent(renderer);
    lastTick = frameStart;
    
    if (!isRunning) {
        #ifdef __EMSCRIPTEN__
        emscripten_cancel_main_loop();
        #endif
    }
}

int main(int argc, char** argv) {
    init();
    
    #ifdef __EMSCRIPTEN__
    // Set the main loop for Emscripten (60 FPS, simulate infinite loop)
    emscripten_set_main_loop(emscripten_main_loop, 60, 1);
    #else
    // Original main loop for native builds
    mainLoop();
    #endif
    
    clean();
    return 0;
}

void init() {
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
    Mix_Init(MIX_INIT_MP3);
    TTF_Init();

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    Mix_AllocateChannels(32);

    window = SDL_CreateWindow("Atlas", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                              1280, 720,
                              SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
        return;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer creation failed: " << SDL_GetError() << std::endl;
        return;
    }

    SDL_ShowCursor(SDL_DISABLE);

    loadResources();

    camera = new Camera(renderer);

    timerGenerate.setOneShot(false);

    timerGenerate.setDuration(1.5f);

    timerGenerate.setOnTimeOut(
        [&]() {
            for (int i = 0; i < numPerGen; i++) {
                int val = rand() % 100;
                Chicken* chicken = nullptr;
                if (val < 50) chicken = new ChickenSlow();
                else if (val < 80) chicken = new ChickenMedium();
                else chicken = new ChickenFast();
                chickenList.push_back(chicken);
            }
        }
    );

    timerIncreaseNumPerGen.setOneShot(false);
    timerIncreaseNumPerGen.setDuration(8.0f);
    timerIncreaseNumPerGen.setOnTimeOut(
        [&]() { numPerGen += 1; }
    );

    animationBarrelFire.setLoop(false);
    animationBarrelFire.setInterval(0.04f);
    animationBarrelFire.setCenter(centerBarrel);
    animationBarrelFire.addFrame(&atlasBarrelFire);
    animationBarrelFire.setOnFinished(
        [&]() { isCooldown = true; }
    );
    animationBarrelFire.setPosition({ 718, 610 });

    Mix_PlayMusic(musicBgm, -1);
}

void clean() {
    delete camera;

    unloadResources();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    TTF_Quit();
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
}

void loadResources() {
    texHeart = IMG_LoadTexture(renderer, "assets/heart.png");
    g_texBullet = IMG_LoadTexture(renderer, "assets/bullet.png");
    texBattery = IMG_LoadTexture(renderer, "assets/battery.png");
    texCrosshair = IMG_LoadTexture(renderer, "assets/crosshair.png");
    texBackground = IMG_LoadTexture(renderer, "assets/background.png");
    texBarrelIdle = IMG_LoadTexture(renderer, "assets/barrel_idle.png");

    atlasBarrelFire.load(renderer, "assets/barrel_fire_%d.png", 3);
    g_atlasChickenFast.load(renderer, "assets/chicken_fast_%d.png", 4);
    g_atlasChickenMedium.load(renderer, "assets/chicken_medium_%d.png", 6);
    g_atlasChickenSlow.load(renderer, "assets/chicken_slow_%d.png", 8);
    g_atlasExplosion.load(renderer, "assets/explosion_%d.png", 5);

    musicBgm = Mix_LoadMUS("assets/bgm.mp3");
    musicLoss = Mix_LoadMUS("assets/loss.mp3");

    soundHurt = Mix_LoadWAV("assets/hurt.wav");
    soundFire1 = Mix_LoadWAV("assets/fire_1.wav");
    soundFire2 = Mix_LoadWAV("assets/fire_2.wav");
    soundFire3 = Mix_LoadWAV("assets/fire_3.wav");
    g_soundExplosion = Mix_LoadWAV("assets/explosionn.wav");

    font = TTF_OpenFont("assets/IPix.ttf", 28);
}

void unloadResources() {
    SDL_DestroyTexture(texHeart);
    SDL_DestroyTexture(g_texBullet);
    SDL_DestroyTexture(texBattery);
    SDL_DestroyTexture(texCrosshair);
    SDL_DestroyTexture(texBackground);
    SDL_DestroyTexture(texBarrelIdle);

    Mix_FreeMusic(musicBgm);
    Mix_FreeMusic(musicLoss);

    Mix_FreeChunk(soundHurt);
    Mix_FreeChunk(soundFire1);
    Mix_FreeChunk(soundFire2);
    Mix_FreeChunk(soundFire3);
    Mix_FreeChunk(g_soundExplosion);
}

void mainLoop() {
    using namespace std::chrono;

    SDL_Event event;

    const nanoseconds frameDuration(100000000 / 144);
    steady_clock::time_point lastTick = steady_clock::now();

    while (isRunning) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    isRunning = false; 
                    break;
                case SDL_MOUSEMOTION: {
                    posCrosshair.x = (float)event.motion.x; 
                    posCrosshair.y = (float)event.motion.y; 
                    Vector2 direction = posCrosshair - posBarrel;
                    angleBarrel = std::atan2(direction.y, direction.x) * 180 / M_PI;
                } 
                break;
                case SDL_MOUSEBUTTONDOWN:
                    isFireKeyDown = true;
                    break;
                case SDL_MOUSEBUTTONUP:
                    isFireKeyDown = false;
                    break;
            }
        }
        steady_clock::time_point frameStart = steady_clock::now();
        duration<float> deltaTime = duration<float>(frameStart - lastTick);

        update(deltaTime.count());

        render(*camera);

        SDL_RenderPresent(renderer);

        lastTick = frameStart;
        nanoseconds sleepDuration = frameDuration - (steady_clock::now() - frameStart);
        if (sleepDuration > nanoseconds(0)) std::this_thread::sleep_for(sleepDuration);
    }
}

void update(float deltaTime) {
    timerGenerate.update(deltaTime);
    timerIncreaseNumPerGen.update(deltaTime);
    
    for (Bullet& bullet : bulletList) bullet.update(deltaTime);

    for (Chicken* chicken : chickenList) {
        chicken->update(deltaTime);

        for (Bullet& bullet : bulletList) {
            if (!chicken->isAlive() || bullet.canRemove()) 
                continue;

            const Vector2& posBullet = bullet.getPosition();
            const Vector2& posChicken = chicken->getPosition();
            static const Vector2 sizeChicken = { 30, 40 };
            if (posBullet.x >= posChicken.x - sizeChicken.x / 2 
                && posBullet.x <= posChicken.x + sizeChicken.x / 2 
                && posBullet.y >= posChicken.y - sizeChicken.y / 2 
                && posBullet.y <= posChicken.y + sizeChicken.y / 2) {
                score += 1;
                bullet.onHit();
                chicken->onHurt();
            }
        }

        if(!chicken->isAlive()) continue;

        if (chicken->getPosition().y >= 720) {
            chicken->makeInvalid();
            Mix_PlayChannel(-1, soundHurt, 0);
            hp -= 1;
        }
    }

    bulletList.erase(
        std::remove_if(bulletList.begin(), bulletList.end(),
        [](const Bullet& bullet) {
            return bullet.canRemove();
        }),
        bulletList.end());

    chickenList.erase(
        std::remove_if(chickenList.begin(), chickenList.end(),
        [](Chicken* chicken) {
            bool canRemove = chicken->shouldRemove();
            if (canRemove) delete chicken;
            return canRemove;
        }),
        chickenList.end());

    // Sorting chickens in the scene based on the Y position
    std::sort(
        chickenList.begin(), chickenList.end(),
        [](const Chicken* chicken1, const Chicken* chicken2) {
            return chicken1->getPosition().y < chicken2->getPosition().y;        
        });

    if (!isCooldown) {
        camera->shake(3.0f, 0.1f);
        animationBarrelFire.update(deltaTime);
    }

    if (isCooldown && isFireKeyDown) {
        animationBarrelFire.reset();
        isCooldown = false;

        static const float lengthBarrel = 105;
        static const Vector2 posBarrelCenter = { 640, 610 };

        bulletList.emplace_back(angleBarrel);
        Bullet& bullet = bulletList.back();
        double angleBullet = angleBarrel + (rand() % 20 - 10); // Bullet angle random change between -15 to 15
        double bulletRadians = angleBullet * M_PI / 180;
        Vector2 bulletdirection = { (float)std::cos(bulletRadians), (float)sin(bulletRadians) };
        bullet.setPosition(posBarrelCenter + bulletdirection * lengthBarrel);

        switch (rand() % 3) {
            case 0: Mix_PlayChannel(-1, soundFire1, 0); break;
            case 1: Mix_PlayChannel(-1, soundFire2, 0); break;
            case 2: Mix_PlayChannel(-1, soundFire3, 0); break;
        }
    }

    camera->update(deltaTime);

    if (hp <= 0) {
        isRunning = false;
        Mix_HaltMusic();
        Mix_PlayMusic(musicLoss, 0);
        std::string msg = "Final Score: " + std::to_string(score);
        
        #ifdef __EMSCRIPTEN__
        // For web, we can't show message boxes, so just log to console
        std::cout << msg << std::endl;
        // Or you could implement a custom game over screen
        #else
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Game Over", msg.c_str(), window);
        #endif
    }
}

void render(const Camera& camera) {
    {
        int widthBg, heightBg;
        SDL_QueryTexture(texBackground, nullptr, nullptr, &widthBg, &heightBg);
        const SDL_FRect rectBg = {
            (1280 - widthBg) / 2.0f,
            (720 - heightBg) / 2.0f,
            (float)widthBg,(float)heightBg
        };
        camera.renderTexture(texBackground, nullptr, &rectBg, 0, nullptr);
    } 

    for (Chicken* chicken : chickenList) 
        chicken->render(camera);

    for (const Bullet& bullet : bulletList) 
        bullet.render(camera);

    {
        int widthBattery, heightBattery;
        SDL_QueryTexture(texBattery, nullptr, nullptr, &widthBattery, &heightBattery);
        const SDL_FRect rectBattery = {
            posBattery.x - widthBattery / 2.0f,
            posBattery.y - widthBattery / 2.0f,
            (float)widthBattery,(float)heightBattery
        };
        camera.renderTexture(texBattery, nullptr, &rectBattery, 0, nullptr);

        int widthBarrel, heightBarrel;
        SDL_QueryTexture(texBarrelIdle, nullptr, nullptr, &widthBarrel, &heightBarrel);
        const SDL_FRect rectBarrel = {
            posBarrel.x, posBarrel.y,
            (float)widthBarrel, (float)heightBarrel
        };
        if (isCooldown) {
            camera.renderTexture(texBarrelIdle, nullptr, &rectBarrel, angleBarrel, &centerBarrel);
        } else {
            animationBarrelFire.setRotation(angleBarrel);
            animationBarrelFire.render(camera);
        }
    }

    {
        int widthHeart, heightHeart;
        SDL_QueryTexture(texHeart, nullptr, nullptr, &widthHeart, &heightHeart);
        for (int i = 0; i < hp; i++) {
            const SDL_Rect rectDst = {
                15 + (widthHeart + 10) * i, 15,
                widthHeart, heightHeart
            };
            SDL_RenderCopy(renderer, texHeart, nullptr, &rectDst);
        }
    }

    {
        std::string strScore = "SCORE " + std::to_string(score);
        SDL_Surface* surfaceScoreBg = TTF_RenderUTF8_Blended(font, strScore.c_str(), { 55, 55, 55, 255 });
        SDL_Surface* surfaceScoreFg = TTF_RenderUTF8_Blended(font, strScore.c_str(), { 255, 255, 255, 255 });
        SDL_Texture* texScoreBg = SDL_CreateTextureFromSurface(renderer, surfaceScoreBg);
        SDL_Texture* texScoreFg = SDL_CreateTextureFromSurface(renderer, surfaceScoreFg);
        SDL_Rect rectDstScore = { 1280 - surfaceScoreBg->w - 15, 15, surfaceScoreBg->w, surfaceScoreBg->h };
        SDL_RenderCopy(renderer, texScoreBg, nullptr, &rectDstScore);
        rectDstScore.x -= 2, rectDstScore.y -= 2;
        SDL_RenderCopy(renderer, texScoreFg, nullptr, &rectDstScore);
        SDL_DestroyTexture(texScoreBg);
        SDL_DestroyTexture(texScoreFg);
        SDL_FreeSurface(surfaceScoreBg);
        SDL_FreeSurface(surfaceScoreFg);
    }

    {
        int widthCrosshair, heightCrosshair;
        SDL_QueryTexture(texCrosshair, nullptr, nullptr, &widthCrosshair, &heightCrosshair);
        const SDL_FRect rectCorsshair = {
            posCrosshair.x - widthCrosshair / 2.0f,
            posCrosshair.y - heightCrosshair / 2.0f,   
            (float)widthCrosshair, (float)heightCrosshair
        };
        camera.renderTexture(texCrosshair, nullptr, &rectCorsshair, 0, nullptr);
    }
}
