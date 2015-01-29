#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "Vector.h"
#include <glm/glm.hpp>

namespace typing
{
    class Camera
    {
    public:
        Camera(const juzutil::Vector3& origin, const juzutil::Vector3& lookat);
        
        void ApplyPerspective();
        const juzutil::Vector2 PerspectiveProject(
                                const juzutil::Vector3& worldCoords) const;
        const juzutil::Vector3 UnPerspectiveProject(
                                const juzutil::Vector2& screenCoords,
                                const float             worldCoordsZ) const;

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
        glm::mat4        m_projection;
        glm::mat4        m_modelview;
    };
}

#endif // __CAMERA_H__
