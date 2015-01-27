#ifndef _B_BOX_H_
#define _B_BOX_H_

#include "Vector.h"

namespace typing
{
    // A 2 dimensional bounding box, all collision tests are only done in 2D.
    class BBox
    {
    public:
        BBox()
        {
        }

        BBox(float minX, float minY, float maxX, float maxY)
            : m_min(minX, minY), m_max(maxX, maxY)
        {
        }

        BBox(const juzutil::Vector2& min, const juzutil::Vector2& max)
            : m_min(min), m_max(max)
        {
        }

        float GetMinX() const
        {
            return m_min[0];
        }

        float GetMinY() const
        {
            return m_min[1];
        }

        float GetMaxX() const
        {
            return m_max[0];
        }

        float GetMaxY() const
        {
            return m_max[1];
        }

        const juzutil::Vector2& GetMin() const
        {
            return m_min;
        }

        const juzutil::Vector2& GetMax() const
        {
            return m_max;
        }

        bool Intersects(const BBox& box) const
        {
            if (m_min[0] > box.m_max[0] || m_max[0] < box.m_min[0])
            {
                return false;
            }

            if (m_min[1] > box.m_max[1] || m_max[1] < box.m_min[1])
            {
                return false;
            }

            return true;
        }

        bool Intersects(const juzutil::Vector2& point) const
        {
            return (m_min[0] < point[0] && m_min[1] < point[1] && m_max[1] > point[1] && m_max[1] > point[1]);
        }

        bool Intersects(const juzutil::Vector3& point) const
        {
            return (m_min[0] < point[0] && m_min[1] < point[1] && m_max[1] > point[1] && m_max[1] > point[1]);
        }

        const BBox operator+(const juzutil::Vector3& origin) const
        {
            return BBox(m_min[0] + origin[0], m_min[1] + origin[1],
                        m_max[0] + origin[0], m_max[1] + origin[1]);
        }

        const BBox operator+(const juzutil::Vector2& origin) const
        {
            return BBox(m_min + origin, m_max + origin);
        }

        const BBox operator-(const juzutil::Vector3& origin) const
        {
            return BBox(m_min[0] - origin[0], m_min[1] - origin[1],
                        m_max[0] - origin[0], m_max[1] - origin[1]);
        }

        const BBox operator-(const juzutil::Vector2& origin) const
        {
            return BBox(m_min - origin, m_max - origin);
        }

        void operator+=(const juzutil::Vector3& origin)
        {
            m_min[0] += origin[0]; m_min[1] += origin[1];
            m_max[0] += origin[0]; m_max[1] += origin[1];
        }

        void operator+=(const juzutil::Vector2& origin)
        {
            m_min += origin;
            m_max += origin;
        }

        void operator-=(const juzutil::Vector3& origin)
        {
            m_min[0] -= origin[0]; m_min[1] -= origin[1];
            m_max[0] -= origin[0]; m_max[1] -= origin[1];
        }

        void operator-=(const juzutil::Vector2& origin)
        {
            m_min -= origin;
            m_max -= origin;
        }

    private:
        juzutil::Vector2 m_min;
        juzutil::Vector2 m_max;
    };
}

#endif // _B_BOX_H_
