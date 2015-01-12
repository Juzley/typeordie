#ifndef _ENTITY_H_
#define _ENTITY_H_

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include "Vector.h"
#include "BBox.h"

namespace typing
{
    class BBox;

    class Entity
    {
    public:
        virtual const juzutil::Vector3& GetOrigin()                 const = 0;
        virtual bool                    IsSolid()                   const = 0;
        virtual char                    GetStartChar()              const = 0;
        virtual bool                    StartsWith(const char c)    const = 0;
        virtual bool                    IsPhraseSingle()            const = 0;
        virtual float                   GetTypingSpeed()            const = 0;
        virtual bool                    Unlink()                    const = 0;

        virtual void                    OnType(char  c,
                                               bool *hit,
                                               bool *phraseFinished) = 0;

        virtual const BBox GetBounds() const
        {
            return BBox();
        }


        virtual void OnCollide()
        {
        }

        // Draw2D
        // Called once a frame to render the 2D aspects of the entity's appearance.
        // This is mainly used for drawing the phrase associated with an entity.
        // Orthographic projection will already have been set up before this is
        // called.
        virtual void Draw2D()
        {
        }

        // Draw3D
        // Called once a frame to render the 3D aspects of the entity's appearance.
        virtual void Draw3D()
        {
        }

        // OnSpawn
        // Called when an entity is added to the game's list of entities. The
        // entity should perform any actions it needs to before updating, such
        // as setting firing timers etc.
        virtual void OnSpawn()
        {
        }

        // Update
        // Called once a frame. The entity should perform any actions needed to update
        // the state of the entity, such as moving or firing.
        virtual void Update()
        {
        }

        // SuppressAwardDisplay
        // Called when the award for an entity's phrase is displayed, to determine whether
        // the award display should be suppressed, for example if the entity will add its
        // own award.
        virtual bool SuppressAwardDisplay()
        {
            return false;
        }
        
        virtual unsigned int GetScore() const
        {
            return 0;
        }
    };
    typedef boost::shared_ptr<Entity> EntityPtr;
    typedef boost::weak_ptr<Entity>   EntityWeakPtr;
}

#endif // _ENTITY_H_
