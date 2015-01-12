#ifndef _APP_H_
#define _APP_H_

#include <memory>
#include <SDL/SDL.h>

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
        static const float       SCREEN_WIDTH;
        static const float       SCREEN_HEIGHT;

        // Methods
        void Init();
        void Run();
        void Shutdown();
        bool IsKeyDown(const SDLKey key);

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

    private:

        // Ctors/Dtors
        App() :
           m_keyState(NULL), m_keyStateValid(false), m_currentTime(0)
        {
        }

        // Members
        Uint8 *m_keyState;
        bool   m_keyStateValid;
        float  m_currentTime;
        bool   m_done;

        // Singleton Implementation
        static std::auto_ptr<App> m_singleton;
    };
    #define APP App::GetApp()
}

#endif // _APP_H_
