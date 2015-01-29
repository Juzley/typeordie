#ifndef _APP_H_
#define _APP_H_

#include <memory>
#include <cstdio>
#include <boost/program_options.hpp>
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
        static const std::string  PRE_RELEASE_STRING;

        enum LogLevel { LOG_ERROR, LOG_DEBUG };

        // Methods
        void Init();
        void Run();
        void Shutdown();
        void ParseOptions(int argc, char *argv[]);

        float GetTime ()
        {
            return m_currentTime;
        }

        template <typename T> T GetOption(const char* option)
        {
            return m_options[option].as<T>();
        }

        int GetScreenWidth()
        {
            return GetOption<int>("width");
        }

        int GetScreenHeight()
        {
            return GetOption<int>("height");
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
        SDL_Window                            *m_window;
        Uint8                                 *m_keyState;
        bool                                   m_keyStateValid;
        float                                  m_currentTime;
        bool                                   m_done;
        boost::program_options::variables_map  m_options;

        // Singleton Implementation
        static std::auto_ptr<App> m_singleton;
    };
    #define APP App::GetApp()
}

#endif // _APP_H_
