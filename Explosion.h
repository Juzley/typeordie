#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

#include <string>
#include <memory>
#include "Vector.h"
#include "Effect.h"
#include "Colour.h"

namespace typing
{
    struct ExplosionFragment
    {
        juzutil::Vector3 m_origin;
        juzutil::Vector3 m_dir;
        float            m_speed;
        float            m_accel;
        float            m_alpha;
    };

    class Explosion : public Effect
    {
    public:
        Explosion(const juzutil::Vector3& origin, const ColourRGBA& colour);

        static void Init();

        void Draw();
        void Update();
        void OnSpawn();
        bool Unlink();

    private:
        // Consts/Enums
        static const std::string  EXPLOSION_SOUND;
        static const std::string  FLARE_TEXTURE;
        static const float        START_SPEED;
        static const float        ACCEL;
        static const float        START_ALPHA;
        static const float        LIFETIME;
        static const float        FRAGMENT_SIZE;
        static const unsigned int FRAGMENTS = 20;
        static const float        FLARE_START_SIZE;
        static const float        FLARE_START_ALPHA;
        static const float        FLARE_EXPAND_SPEED;
        static const float        FLARE_ALPHA_FADE;

        // Members
        juzutil::Vector3  m_origin;
        ColourRGBA        m_colour;
        ExplosionFragment m_fragments[FRAGMENTS];
        float             m_age;
        float             m_fadeSpeed;
    };
    typedef std::shared_ptr<Explosion> ExplosionPtr;
}

#endif // _EXPLOSION_H_
