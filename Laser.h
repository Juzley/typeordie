#ifndef _LASER_H_
#define _LASER_H_

#include <string>
#include <memory>
#include "Vector.h"
#include "Effect.h"
#include "Colour.h"

namespace typing
{
    class Laser : public Effect
    {
    public:
        // Ctors/Dtors
        Laser(const juzutil::Vector3& start, const juzutil::Vector3& end, const ColourRGB& col)
            : m_age(0), m_start(start), m_end(end), m_col(col)
        {
        }

        // Methods
        static void Init();

        void Draw();
        void Update();
        bool Unlink();
        void OnSpawn();

    private:
        // Conts
        static const std::string LASER_SOUND;
        static const float       LINE_DRAW_TIME;
        static const float       LINE_FADE_TIME;
        static const float       LIFETIME;

        // Members
        float            m_age;
        juzutil::Vector3 m_start;
        juzutil::Vector3 m_end;
        ColourRGB        m_col;
    };
    typedef std::shared_ptr<Laser> LaserPtr;
}

#endif // _LASER_H_
