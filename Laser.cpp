#include "Laser.h"
#include "Game.h"
#include "TextureManager.h"
#include "SoundManager.h"
#include "Utils.h"

namespace typing
{
    const std::string Laser::LASER_SOUND("sounds/laser.wav");
    const float       Laser::LINE_DRAW_TIME  = 0.0f;
    const float       Laser::LINE_FADE_TIME  = 0.5f;
    const float       Laser::LIFETIME = LINE_DRAW_TIME + LINE_FADE_TIME;

    void Laser::Init()
    {
        SOUNDS.Add(LASER_SOUND);
    }

    void Laser::Draw()
    {
        float alpha;
        if (m_age < LINE_DRAW_TIME) {
            alpha = 1.0f;
        } else {
            alpha = 1.0f - (m_age - LINE_DRAW_TIME) / LINE_FADE_TIME;
        }

        DrawLine(ColourRGBA(m_col, alpha), m_start, m_end);

        glLineWidth(4.0f);
        DrawLine(ColourRGBA(m_col, alpha / 2.0f), m_start, m_end);
        glLineWidth(1.0f);
    }

    void Laser::Update()
    {
        m_age += GAME.GetFrameTime();
    }

    bool Laser::Unlink()
    {
        return (m_age >= LIFETIME);
    }

    void Laser::OnSpawn()
    {
        SOUNDS.Play(LASER_SOUND);
    }
}
