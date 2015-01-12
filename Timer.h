#ifndef _TIMER_H_
#define _TIMER_H_

#include "App.h"

namespace typing
{
    class Timer
    {
    public:
        // Ctors/Dtors
        Timer () :
            m_lastTime(0), m_frameTime(0), m_time(0), m_paused(false)
        {
        }

        // Methods
        void Reset()
        {
            m_lastTime  = APP.GetTime();
            m_frameTime = 0;
            m_time      = 0;
            m_paused    = false;
        }

        void Update()
        {
            float thisTime = APP.GetTime();

            if (!m_paused)
            {
                m_frameTime = thisTime - m_lastTime;
                m_time += m_frameTime;
            }

            m_lastTime = thisTime;
        }

        float GetFrameTime() const
        {
            return m_frameTime;
        }

        float GetTime() const
        {
            return m_time;
        }

        void Pause (bool pause)
        {
            m_paused = pause;
        }

    private:
        float m_lastTime;
        float m_frameTime;
        float m_time;
        bool  m_paused;
    };
}

#endif // _TIMER_H_
