#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <string>
#include "Vector.h"
#include "BBox.h"
#include "Timer.h"

namespace typing
{
    class Player
    {
    public:
        static void Init();

        void Reset(unsigned int lives);
        void Draw();
        void Update();
        void Fire();
        void Damage();

        const juzutil::Vector3& GetOrigin() const
        {
            return PLAYER_ORIGIN;
        }

        const BBox GetBounds() const
        {
            return PLAYER_BOUNDS + PLAYER_ORIGIN;
        }

        unsigned int Lives() const
        {
            return m_lives;
        }

        void ExtraLife()
        {
            m_lives++;
        }

    private:
        // Consts/Enums
        static const float            FIRE_FADE_TIME;
        static const float            DAMAGE_FADE_TIME;
        static const float            PLAYER_SIDE;
        static const juzutil::Vector3 PLAYER_ORIGIN;
        static const BBox             PLAYER_BOUNDS;

        // Members
        unsigned int m_lives;
        float        m_lastFireTime;
        float        m_damageTime;
    };
}

#endif // _PLAYER_H_
