#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include "Utils.h"
#include "Powerup.h"
#include "Game.h"
#include "TextureManager.h"
#include "Explosion.h"
#include "Shape.h"
#include "Random.h"
#include "Award.h"

namespace typing
{
    static const float      POWERUP_LIFETIME = 4.0f;
    static const float      POWERUP_BLINK_TIME = 1.0f;
    static const float      POWERUP_BLINK_SPEED = 8.0f;
    static const ColourRGBA POWERUP_SPHERE_COLOUR(1.0f, 1.0f, 1.0f, 0.3f);

    //////////////////////////////////////////////////////////////////////////
    // PowerupFactory
    //////////////////////////////////////////////////////////////////////////

    PowerupPtr PowerupFactory::Create(const juzutil::Vector3& origin)
    {
        if (m_creators.empty()) {
            throw std::runtime_error("Empty Powerup Factory");
        } else {
            unsigned int index = RAND.Range(static_cast<unsigned int>(0),
                                            static_cast<unsigned int>(m_creators.size() - 1));
            PowerupCreator create = m_creators.at(index);
            return create(GAME.GetPhrase(PhraseBook::PL_LONG), origin);
        }
    }


    //////////////////////////////////////////////////////////////////////////
    // ExtraLife
    //////////////////////////////////////////////////////////////////////////
        
    void ExtraLife::OnType(char c, bool *hit, bool *phraseFinished)
    {
        *hit = m_phrase.OnType(c, GAME.GetTime());


        if (!*hit) {
            GAME.MakeCharAvail(m_phrase.GetStartChar());
            m_unlink = true;
        } else {
            *phraseFinished = m_phrase.Finished();

            if (*phraseFinished) {
                GAME.MakeCharAvail(m_phrase.GetStartChar());
                GAME.AddExtraLife();
                // TODO: Offset award display
                GAME.AddEffect2d(
                    AwardPtr(new Award(GAME.GetCam().PerspectiveProject(m_origin), AWARD_EXTRALIFE, GAME.GetTime())));
                m_unlink = true;
            }
        }
    }

    void ExtraLife::Draw2D()
    {
        m_phrase.Draw(m_origin);
    }

    void ExtraLife::Draw3D()
    {
        ColourRGBA sphereColour(POWERUP_SPHERE_COLOUR);
        const float blinkTime = (GAME.GetTime() - m_spawnTime) - (POWERUP_LIFETIME - POWERUP_BLINK_TIME);
        if (blinkTime > 0.0f) {
            sphereColour[ColourRGBA::COLOUR_ALPHA] = POWERUP_SPHERE_COLOUR.GetAlpha() *
                abs(cosf((blinkTime / POWERUP_BLINK_TIME) * static_cast<float>(M_PI) * POWERUP_BLINK_SPEED));
        }

        glPushMatrix();
            glTranslatef(m_origin[0], m_origin[1], m_origin[2]);
            glScalef(20.0f, 20.0f, 20.0f);
            DrawSphere(ColourRGBA(sphereColour));
        glPopMatrix();
    }

    void ExtraLife::OnSpawn()
    {
        m_spawnTime = GAME.GetTime();
    }

    void ExtraLife::Update()
    {
        if (GAME.GetTime() - m_spawnTime >= POWERUP_LIFETIME) {
            GAME.MakeCharAvail(GetStartChar());
            m_unlink = true;
        }
    }
}
