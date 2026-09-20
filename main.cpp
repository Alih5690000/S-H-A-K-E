#include <emscripten/emscripten.h>
#include <SDL2/SDL.h>
#include <iostream>

int randint(int a, int b){
    int num=rand();
    int c=abs(a-b);
    int m=std::min(a,b);
    return (rand()%c)+m;
}

float randint(float a, float b){
    int num=rand();
    int c=abs(a-b);
    float m=std::min(a,b);
    return (rand()%c)+m;
}

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Texture* txt;
float offsetX, offsetY;

template <typename T>
struct Vec2{
    T x,y;
};
typedef Vec2<float> Vec2f;

Vec2f getOffset(float intensivity){
    Vec2f vec={randint(-intensivity/2,intensivity/2),
        randint(-intensivity/2,intensivity/2)};
    return vec;
}

bool shaking=false;
bool fPressed=false;

void loop(){
    SDL_Event e;
    while(SDL_PollEvent(&e)){
        if (e.type==SDL_QUIT) emscripten_cancel_main_loop();
    }

    const Uint8* keys=SDL_GetKeyboardState(NULL);
    if (keys[SDL_SCANCODE_F] && !fPressed){
        fPressed=true;
        shaking=!shaking;
    }
    else{
        fPressed=false;
    }

    SDL_SetRenderTarget(renderer, txt);

    SDL_FRect rect={500, 400, 100, 100};
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_RenderFillRectF(renderer, &rect);

    SDL_SetRenderTarget(renderer, NULL);
    SDL_FRect drawRect={0, 0, 1000, 800};
    if (shaking){
        Vec2f v=getOffset(10);
        drawRect.x=v.x;
        drawRect.y=v.y;
    }
    SDL_RenderCopyF(renderer, txt, NULL, &drawRect);

    SDL_RenderPresent(renderer);
}

int main(){
    SDL_Init(SDL_INIT_EVERYTHING);
    window=SDL_CreateWindow("lol", SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, 1000, 800, SDL_WINDOW_SHOWN);
    renderer=SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    txt=SDL_CreateTexture(renderer, 
        SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, 1000, 800);
    emscripten_set_main_loop(loop, 0, 1);
}