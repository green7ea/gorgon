#include <SDL.h>
#include <GL/gl.h>
#include <string>

// for open close
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "model.h"

void init()
{
    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClearDepth(1.f);
    glEnable(GL_DEPTH_TEST);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

bool handle_events()
{
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0)
    {
        if (event.type == SDL_QUIT)
        {
            return false;
        }
        else if (event.type == SDL_KEYDOWN)
        {
            switch (event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                return false;
                break;
            default:
                break;
            }
        }
    }

    return true;
}

void draw(Model &model)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glScalef(0.3f, 0.3f, 0.3f);

    glColor3f(0.5f, 0.5f, 0.5f);

    model.draw();
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("Usage:\n"
               "%s [modelname]\n",
               argv[0]);
        return 1;
    }

    std::string filename(argv[1]);

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        return 2;
    }

    auto window = SDL_CreateWindow("Example Gorgon Load",
                                   SDL_WINDOWPOS_CENTERED,
                                   SDL_WINDOWPOS_CENTERED,
                                   800, 600,
                                   SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
        return 3;
    }

    auto context = SDL_GL_CreateContext(window);

    if (SDL_GL_MakeCurrent(window, context) != 0)
    {
        return 4;
    }

    init();

    int fd = open(filename.c_str(), O_RDONLY);
    if (fd < 0)
    {
        fprintf(stderr, "Couldn't open the model: %s\n", filename.c_str());
        return 5;
    }
    Model mesh(fd);
    close(fd);

    bool running = true;
    while (running)
    {
        running = handle_events();
        draw(mesh);
        SDL_GL_SwapWindow(window);
    }

    SDL_GL_DeleteContext(context);
    context = NULL;
    SDL_DestroyWindow(window);
    window = NULL;
    SDL_Quit();

    return 0;
}
