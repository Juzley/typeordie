#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "Vector.h"

namespace typing
{
    class Camera
    {
    public:
        Camera(const juzutil::Vector3& origin, const juzutil::Vector3& lookat);
        
        void ApplyPerspective();
        const juzutil::Vector2 PerspectiveProject(const juzutil::Vector3& worldCoords) const;

        void ApplyOrtho() const;

        const juzutil::Vector3& GetUp() const
        {
            return (m_up);
        }

        const juzutil::Vector3& GetRight() const
        {
            return (m_right);
        }

    private:
        juzutil::Vector3 m_origin;
        juzutil::Vector3 m_lookat;
        juzutil::Vector3 m_forward;
        juzutil::Vector3 m_right;
        juzutil::Vector3 m_up;
        double           m_projection[16];
        double           m_modelview[16];
    };
}

#endif // __CAMERA_H__
