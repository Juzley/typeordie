#include <math.h>
#include "Matrix.h"

namespace juzutil
{
    //////////////////////////////////////////////////////////////////////////
    //
    // MATRIX2
    //
    //////////////////////////////////////////////////////////////////////////

    bool Matrix2::Inverse(Matrix2& mat) const
    {
        double det = m_matrix[0][0] * m_matrix[1][1] - m_matrix[1][0] * m_matrix[0][1];
        if(fabsf(det) < MATRIX_INVERSE_EPSILON)
        {
            return false;
        }

        double inv = 1.0 / det;        
        mat[0][0] =  m_matrix[1][1] * static_cast<float>(inv);
        mat[1][1] =  m_matrix[0][0] * static_cast<float>(inv);
        mat[0][1] = -m_matrix[0][1] * static_cast<float>(inv);
        mat[1][0] = -m_matrix[1][0] * static_cast<float>(inv);
        return true;
    }

    inline const Matrix3 Matrix2::ToMatrix3() const
    {
        return Matrix3(m_matrix[0][0], m_matrix[0][1], 0, 
                       m_matrix[1][0], m_matrix[1][1], 0,
                       0, 0, 1);
    }

    inline const Matrix4 Matrix2::ToMatrix4() const
    {
        return Matrix4(m_matrix[0][0], m_matrix[0][1], 0, 0,
                       m_matrix[1][0], m_matrix[1][1], 0, 0,
                       0, 0, 1, 0,
                       0, 0, 0, 1);
    }

    //////////////////////////////////////////////////////////////////////////
    //
    // MATRIX3
    //
    //////////////////////////////////////////////////////////////////////////

    bool Matrix3::Inverse(Matrix3& mat) const
    {
        mat[0][0] = m_matrix[1][1] * m_matrix[2][2] - m_matrix[1][2] * m_matrix[2][1];
        mat[1][0] = m_matrix[1][2] * m_matrix[2][0] - m_matrix[1][0] * m_matrix[2][2];
        mat[2][0] = m_matrix[1][0] * m_matrix[2][1] - m_matrix[1][1] * m_matrix[2][0];

        double det = m_matrix[0][0] * mat[0][0] + m_matrix[0][1] * mat[1][0] + m_matrix[0][2] * mat[2][0];

        if(fabsf(det) < MATRIX_INVERSE_EPSILON)
        {
            return false;
        }

        double inv = 1.0 / det;

        mat[0][1] = m_matrix[0][2] * m_matrix[2][1] - m_matrix[0][1] * m_matrix[2][2];
        mat[0][2] = m_matrix[0][1] * m_matrix[1][2] - m_matrix[0][2] * m_matrix[1][1];
        mat[1][1] = m_matrix[0][0] * m_matrix[2][2] - m_matrix[0][2] * m_matrix[2][0];
        mat[1][2] = m_matrix[0][2] * m_matrix[1][0] - m_matrix[0][0] * m_matrix[1][2];
        mat[2][1] = m_matrix[0][1] * m_matrix[2][0] - m_matrix[0][0] * m_matrix[2][1];
        mat[2][2] = m_matrix[0][0] * m_matrix[1][1] - m_matrix[0][1] * m_matrix[1][0];

        mat *= static_cast<float>(inv);
        return true;
    }

    const Matrix4 Matrix3::ToMatrix4() const
    {
        return Matrix4(m_matrix[0][0], m_matrix[0][1], m_matrix[0][2], 0,
                       m_matrix[1][0], m_matrix[1][1], m_matrix[1][2], 0,
                       m_matrix[2][0], m_matrix[2][1], m_matrix[2][2], 0,
                       0, 0, 0, 1);
    }

    //////////////////////////////////////////////////////////////////////////
    //
    // MATRIX4
    //
    //////////////////////////////////////////////////////////////////////////

    bool Matrix4::Inverse(Matrix4&) const
    {
        // TODO: meh
        return false;
    }
}
