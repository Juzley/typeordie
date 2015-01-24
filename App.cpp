#include <memory>
#include <stdexcept>
#include <boost/program_options.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_mixer.h>
#include "App.h"
#include "Game.h"
#include "Menu.h"
#include "HighScores.h"

namespace typing
{
    const unsigned int App::MAJOR_VERSION  = 1;
    const unsigned int App::MINOR_VERSION  = 0;

    std::auto_ptr<App> App::m_singleton(new App());
    App& App::GetApp ()
    {
        return *(m_singleton.get());
    }

    void App::ParseOptions (int argc, char *argv[])
    {
        namespace po = boost::program_options;

        po::options_description desc("Allowed options");
        desc.add_options()
            ("width,w",
                po::value<int>()->default_value(800), "set resolution width")
            ("height,h",
                po::value<int>()->default_value(600), "set resolution height")
            ("fullscreen,f",
                po::bool_switch(), "use full-screen")
            ("text-scale,t",
                po::value<double>()->default_value(1.0),
                "set in-game text scale")
        ;

        po::store(po::parse_command_line(argc, argv, desc), m_options);
        po::notify(m_options);
    }


    void App::Init ()
    {
        // Initialise SDL
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
        {
            // TODO: throw
        }

        // Initialise video
        m_window = SDL_CreateWindow("Type Or Die!",
                                    SDL_WINDOWPOS_UNDEFINED,
                                    SDL_WINDOWPOS_UNDEFINED,
                                    GetOption<int>("width"),
                                    GetOption<int>("height"),
                                    SDL_WINDOW_OPENGL |
                                    GetOption<bool>("fullscreen") ?
                                        (SDL_WINDOW_OPENGL |
                                         SDL_WINDOW_FULLSCREEN) :
                                        SDL_WINDOW_OPENGL);
        if (!m_window) {
            // TODO: throw
        }

        if (!SDL_GL_CreateContext(m_window)) {
            // TODO: throw
        }

        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);

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

            SDL_StartTextInput();
            while(SDL_PollEvent(&ev))
            {
                switch(ev.type)
                {
                case SDL_KEYDOWN:
                    MENU.OnKeyDown(ev.key.keysym.sym);

                    if (!MENU.IsActive()) {
                        GAME.OnKeyDown(ev.key.keysym.sym);
                    }
                    break;

                case SDL_TEXTINPUT:
                    for (char *c = ev.text.text; *c != '\0'; c++) {
                        MENU.OnType(*c);

                        if (!MENU.IsActive()) {
                            GAME.OnType(*c);
                        }
                    }
                    break;

                case SDL_QUIT:
                    m_done = true;
                    break;
                }
            }

            GAME.Update();
            MENU.Update();

            glClear(GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |
                    GL_COLOR_BUFFER_BIT);

            GAME.Draw();

            // All the menu stuff is done in orthographic projection.
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho(0.0, GetScreenWidth(), GetScreenHeight(), 0.0,
                    1024.0, -1024.0);
            MENU.Draw();

            SDL_GL_SwapWindow(m_window);

            // Prepare for the next frame
            m_keyStateValid = false;
        }
    }

    void App::Shutdown ()
    {
        Mix_CloseAudio();
        SDL_Quit();
    }
}

int main (int argc, char *argv[])
{
    try {
        typing::APP.ParseOptions(argc, argv);
        typing::APP.Init();
        typing::APP.Run();
    } catch (std::exception &e) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
                                 "Exception Caught",
                                 e.what(),
                                 NULL);
    }

    typing::APP.Shutdown();

    return 0;
}

