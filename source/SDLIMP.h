#pragma once

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Texture;


class SDLIMP
{
    private:
        SDL_Window* window{};
        SDL_Renderer* renderer{};
        SDL_Texture* texture{};

    public:
        SDLIMP(char const* title, int windowWidth, int windowHeight, int tWidth, int tHeight);
        ~SDLIMP();
        bool UpdateKeys(unsigned short* keys);
        void UpdateScreen(void const* buffer, int pitch);

};



