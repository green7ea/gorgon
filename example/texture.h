#pragma once

#include <GL/gl.h>
#include <SDL.h>

class Texture
{

public:
    Texture()
        : texture(-1)
    {

    }

    ~Texture()
    {
        if (texture > 0)
        {
            glDeleteTextures(1, &texture);
        }
    }

    bool load(const std::string &filepath)
    {
        SDL_Surface *surface = SDL_LoadBMP(filepath.c_str());

        if (!surface)
        {
            fprintf(stderr, "Couldn't load %s. This example code only support BMPs.\n",
                    filepath.c_str());
            return false;
        }

        glGenTextures(1, &texture);

        glBindTexture(GL_TEXTURE_2D, texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        const int number_of_colors = surface->format->BytesPerPixel;
        int texture_format;
        if (number_of_colors == 4)
        {
            if (surface->format->Rmask == 0x000000ff)
            {
                texture_format = GL_RGBA;
            }
            else
            {
                texture_format = GL_BGRA;
            }
        }
        else if (number_of_colors == 3)
        {
            if (surface->format->Rmask == 0x000000ff)
            {
                texture_format = GL_RGB;
            }
            else
            {
                texture_format = GL_BGR;
            }
        }
        else
        {
            SDL_FreeSurface(surface);
            return false;
        }

        glTexImage2D(GL_TEXTURE_2D, 0, number_of_colors,
                     surface->w, surface->h, 0, texture_format,
                     GL_UNSIGNED_BYTE, surface->pixels);

        if (surface)
        {
            SDL_FreeSurface(surface);
        }

        return true;
    }

    GLuint texture;
};
