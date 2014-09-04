#include <iostream>
#include <thread>
#include <SDL2/SDL.h>

class Renderer
{
public:
    Renderer(SDL_Window* window);

private:
    void renderJob();
    void render();

    SDL_GLContext mContext;
    SDL_Renderer* mRenderer;
    SDL_Window* mWindow;
    bool mFirstRun;

};

Renderer::Renderer(SDL_Window* window)
    : mWindow(window),
      mRenderer(nullptr),
      mFirstRun(true)
{
    mContext = SDL_GL_GetCurrentContext();

    SDL_GL_MakeCurrent(window, nullptr);

    std::thread t(&Renderer::renderJob, this);

    t.detach();
}


void Renderer::render()
{
    // Clear
    if (SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255) < 0)
    {
        std::cerr << "Error clearing: " << SDL_GetError() << std::endl;
    }

    // Draw
    SDL_Rect rect;
    rect.x = 10;
    rect.y = 10;
    rect.w = 50;
    rect.h = 50;

    SDL_SetRenderDrawColor(mRenderer, 255, 0, 0, 255);

    if (SDL_RenderDrawRect(mRenderer, &rect) < 0)
    {
        std::cerr << "Error drawing: " << SDL_GetError() << std::endl;
    }

    // Display
    SDL_RenderPresent(mRenderer);
}

void Renderer::renderJob()
{
    while (true)
    {
        if (mFirstRun)
        {
            SDL_GL_MakeCurrent(mWindow, mContext);

            mFirstRun = false;
            mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);
        }
        if (mRenderer != nullptr)
        {
            std::cout << "RENDERING!" << std::endl;
            render();
        }
    }
}

int main()
{
    SDL_Init(SDL_INIT_VIDEO);
    auto window = SDL_CreateWindow("Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 500, 500, SDL_WINDOW_RESIZABLE);

    Renderer r(window);


    SDL_Delay(5000);

    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}
