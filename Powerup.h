#ifndef _POWERUP_H_
#define _POWERUP_H_

#include <ctime>
#include <stdexcept>
#include <vector>
#include <memory>
#include <math.h>
#include "Entity.h"
#include "Powerup.h"
#include "Vector.h"
#include "Phrase.h"

namespace typing
{
    //////////////////////////////////////////////////////////////////////////
    // Powerup
    //////////////////////////////////////////////////////////////////////////
    class Powerup : public Entity
    {
    public:        
        virtual const juzutil::Vector3& GetOrigin()                 const = 0;
        virtual char                    GetStartChar()              const = 0;
        virtual bool                    StartsWith(const char c)    const = 0;
        virtual bool                    IsPhraseSingle()            const = 0;
        virtual float                   GetTypingSpeed()            const = 0;
        virtual bool                    Unlink()                    const = 0;

        virtual void                    OnType(char c,
                                               bool *hit,
                                               bool *phraseFinished) = 0;

        bool IsSolid() const
        {
            return (false);
        }

        virtual const BBox GetBounds() const
        {
            return Entity::GetBounds();
        }

        virtual void OnSpawn()
        {
        }

        virtual void Update()
        {
        }

        virtual void Draw2D()
        {
        }

        virtual void Draw3D()
        {
        }

        virtual bool SuppressAwardDisplay()
        {
            return true;
        }

    };
    typedef std::shared_ptr<Powerup> PowerupPtr;
    typedef PowerupPtr (*PowerupCreator)(const std::string& phrase, const juzutil::Vector3& origin);

    template<typename powerupType>
    PowerupPtr CreatePowerup(const std::string& phrase, const juzutil::Vector3& origin)
    {
        return PowerupPtr(new powerupType(phrase, origin));
    }


    //////////////////////////////////////////////////////////////////////////
    // PowerupFactory
    //////////////////////////////////////////////////////////////////////////
    class PowerupFactory
    {
    public:
        // Methods
        void Register(PowerupPtr (*creator)(const std::string& phrase, const juzutil::Vector3& origin))//PowerupCreator creator)
        {
            m_creators.push_back(creator);
        }

        PowerupPtr Create(const juzutil::Vector3& origin);

    private:
        // Typedefs
        typedef std::vector<PowerupCreator> PowerupCreationVec;

        // Members
        PowerupCreationVec m_creators;
    };


    //////////////////////////////////////////////////////////////////////////
    // ExtraLife
    //////////////////////////////////////////////////////////////////////////
    class ExtraLife : public Powerup
    {
    public:
        ExtraLife (const std::string& phrase, const juzutil::Vector3& origin)
            : m_phrase(phrase), m_origin(origin), m_unlink(false)
        {
        }

        const juzutil::Vector3& GetOrigin() const
        {
            return m_origin;
        }

        char GetStartChar() const
        {
            return m_phrase.GetStartChar();
        }

        bool StartsWith(const char c) const
        {
            return (GetStartChar() == c);
        }

        bool IsPhraseSingle() const
        {
            return (m_phrase.Length() == 1);
        }

        float GetTypingSpeed() const
        {
            return (m_phrase.GetTypingSpeed());
        }

        bool Unlink () const
        {
            return m_unlink;
        }

        void OnSpawn();
        void Update();
        void OnType(char c, bool *hit, bool *phraseFinished);
        void Draw2D();
        void Draw3D();    

    private:
        Phrase           m_phrase;
        juzutil::Vector3 m_origin;
        bool             m_unlink;
        float            m_spawnTime;
    };
}

#endif // _POWERUP_H_
