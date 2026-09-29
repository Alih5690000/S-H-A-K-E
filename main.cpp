#include <emscripten/emscripten.h>
#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <cmath>

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

class Sprite{
    public:
    SDL_FRect rect;
    bool active=true;
    float hp;
    bool alive=true;
    std::vector<Sprite*>& sprites;
    Sprite(std::vector<Sprite*>& s):sprites(s){}
    virtual void update(SDL_Renderer* r, float){
        SDL_SetRenderDrawColor(r, 0, 255, 0, 255);
        SDL_RenderFillRectF(renderer, &rect);
    };
    virtual ~Sprite()=default;
};

void UpdateList(std::vector<Sprite*>& s, SDL_Renderer* r, float dt){
        std::vector<Sprite*> snew;
        for (auto i:s){
            i->update(r, dt);
        }
        for (auto i:s){
            if (i->active) snew.push_back(i);
            else delete i;
        }
        s=snew;
    }

typedef std::vector<Sprite*> Sprites;

class Bullet:public Sprite{
    public:
    float dx,dy;
    Sprite* shooter;
    bool first=true;
    Bullet(float dirX, float dirY, std::vector<Sprite*>& s, Sprite* ss, SDL_FRect r):
        Sprite(s), dx(dirX), dy(dirY){
            if (ss) shooter=ss;
            rect=r;
        }
    void update(SDL_Renderer* r, float dt){
        rect.x+=dx*dt;
        rect.y+=dy*dt;

        for (auto i:sprites){
            if (SDL_HasIntersectionF(&i->rect, &rect)){
                if (shooter && i!=shooter && !first)
                    i->active=false;
                first=false;
            }
        }

        SDL_SetRenderDrawColor(r, 255, 0, 0, 255);
        SDL_RenderFillRectF(r, &rect);
    }
};

bool shaking=false;
bool fPressed=false;
int start, end;
float dt;
SDL_FRect rect={500, 400, 100, 100};
Sprites ss={new Sprite(ss)};

void loop(){
    start=SDL_GetTicks();
    dt=(start-end)/1000.f;
    end=start;
    SDL_Event e;
    while(SDL_PollEvent(&e)){
        if (e.type==SDL_QUIT) emscripten_cancel_main_loop();
    }

    shaking=false;
    const Uint8* keys=SDL_GetKeyboardState(NULL);
    if (keys[SDL_SCANCODE_W]){
        rect.y-=200*dt;
        shaking=true;
    }
    if (keys[SDL_SCANCODE_S]){
        rect.y+=200*dt;
        shaking=true;
    }
    if (keys[SDL_SCANCODE_A]){
        rect.x-=200*dt;
        shaking=true;
    }
    if (keys[SDL_SCANCODE_D]){
        rect.x+=200*dt;
        shaking=true;
    }
    int x, y;
    const Uint32 mstate=SDL_GetMouseState(&x, &y);
    if (mstate & SDL_BUTTON_LMASK){
        float dx = x - rect.x;
        float dy = y - rect.y;

        float len = std::sqrt(dx*dx + dy*dy);

        dx /= len;
        dy /= len;

        ss.push_back(new Bullet(dx * 500, dy * 500, ss, nullptr, {rect.x, rect.y, 10, 10}));
    }

    SDL_SetRenderTarget(renderer, txt);

    SDL_SetRenderDrawColor(renderer, 0 ,0 ,0, 255);
    SDL_RenderClear(renderer);

    UpdateList(ss, renderer, dt);

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
    ss[0]->rect={0,0,100,100};
    emscripten_set_main_loop(loop, 0, 1);
}