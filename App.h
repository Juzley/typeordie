#ifndef _APP_H_
#define _APP_H_

#include <memory>
#include <cstdio>
#include <SDL2/SDL.h>

namespace typing
{
    class App
    {
    public:
        // Singleton Implementation
        static App& GetApp();

        // Consts/Enums
        static const unsigned int MAJOR_VERSION;
        static const unsigned int MINOR_VERSION;
        static const float        SCREEN_WIDTH;
        static const float        SCREEN_HEIGHT;

        enum LogLevel { LOG_ERROR, LOG_DEBUG };

        // Methods
        void Init();
        void Run();
        void Shutdown();

        float GetTime ()
        {
            return m_currentTime;
        }

        float GetScreenWidth()
        {
            return SCREEN_WIDTH;
        }

        float GetScreenHeight()
        {
            return SCREEN_HEIGHT;
        }

        void Quit()
        {
            m_done = true;
        }

        void Log (LogLevel           level,
                  const std::string &str)
        {
            switch (level) {
            case LOG_DEBUG:
#ifdef _DEBUG
                fprintf(stdout, "%s\n", str.c_str());
#endif 
                break;

            case LOG_ERROR:
                fprintf(stderr, "%s\n", str.c_str());
                break;
            }
        }

    private:

        // Ctors/Dtors
        App() :
           m_keyState(NULL), m_keyStateValid(false), m_currentTime(0)
        {
        }

        // Members
        SDL_Window *m_window;
        Uint8      *m_keyState;
        bool        m_keyStateValid;
        float       m_currentTime;
        bool        m_done;

        // Singleton Implementation
        static std::auto_ptr<App> m_singleton;
    };
    #define APP App::GetApp()
}

#endif // _APP_H_
