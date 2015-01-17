#include <memory>
#include <stdexcept>
#include <SDL/SDL_opengl.h>
#include <SDL/SDL_mixer.h>
#include "App.h"
#include "Game.h"
#include "Menu.h"
#include "HighScores.h"

namespace typing
{
    const unsigned int App::MAJOR_VERSION  = 1;
    const unsigned int App::MINOR_VERSION  = 0;
    const float App::SCREEN_WIDTH          = 800.0f;
    const float App::SCREEN_HEIGHT         = 600.0f;

    std::auto_ptr<App> App::m_singleton(new App());
    App& App::GetApp ()
    {
        return *(m_singleton.get());
    }

    void App::Init ()
    {
        // Initialise SDL
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
        {
            // TODO: throw
        }

        // Initialise video
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);

        if (!SDL_SetVideoMode(800, 600, 32, SDL_OPENGL))
        {
            // TODO: throw
        }

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClearDepth(1.0f);

        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
        glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
        glEnable(GL_LINE_SMOOTH);
        glDisable(GL_LIGHTING);
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glMatrixMode(GL_PROJECTION_MATRIX);
        glOrtho(0.0, GetScreenWidth(), GetScreenHeight(), 0.0, 1024.0, -1024.0);
        glMatrixMode(GL_MODELVIEW_MATRIX);

        // Initialise audio
        int     audio_rate     = 22050;
        Uint16  audio_format   = AUDIO_S16;
        int     audio_channels = 2;
        int     audio_buffers  = 4096;
        if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers))
        {
            // TODO: throw
        }

        (void)SDL_EnableUNICODE(1);

        SDL_WM_SetCaption("Type Or Die!", NULL);

        // Initialise game stuff
        SCORES.Load();
        MENU.Init();
        GAME.Init();
    }

    void App::Run ()
    {
        SDL_Event ev;

        m_done = false;
        while (!m_done)
        {
            // Get the time of this frame
            m_currentTime = static_cast<float>(SDL_GetTicks()) / 1000.0f;

            while(SDL_PollEvent(&ev))
            {
                switch(ev.type)
                {
                case SDL_KEYDOWN:
                    MENU.OnType(ev.key.keysym);

                    if (!MENU.IsActive()) {
                        GAME.OnType(ev.key.keysym);
                    }
                    break;

                case SDL_QUIT:
                    m_done = true;
                    break;
                }
            }

            GAME.Update();
            MENU.Update();

            glClear(GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

            GAME.Draw();

            // All the menu stuff is done in orthographic projection.
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho(0.0, GetScreenWidth(), GetScreenHeight(), 0.0, 1024.0, -1024.0);
            MENU.Draw();

            SDL_GL_SwapBuffers();

            // Prepare for the next frame
            m_keyStateValid = false;
        }
    }

    void App::Shutdown ()
    {
        Mix_CloseAudio();
        SDL_Quit();
    }

    bool App::IsKeyDown (const SDLKey key)
    {
        if (!m_keyStateValid)
        {
            m_keyState = SDL_GetKeyState(NULL);
            m_keyStateValid = true;
        }

        return (m_keyState[key] == 1);
    }
}

int main ()
{
    try
    {
        typing::APP.Init();
        typing::APP.Run();
    }
    catch (std::exception &e)
    {
        fprintf(stderr, "%s", e.what());
    }

    typing::APP.Shutdown();

    return 0;
}

